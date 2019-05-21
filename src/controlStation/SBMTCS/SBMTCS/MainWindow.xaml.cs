using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml.Serialization;
using System.Diagnostics;
using System.IO;
using System.Collections.ObjectModel;
using System.Timers;

namespace SBMTCS
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
		AdvancedPortScanner portScanner;
		SerialLink link;
		Config config;

		ObservableCollection<SensorData> sensorDatas;

		Timer secTick;
		DateTime localTime;
		DateTime remoteTime;

		bool isLogging;

		StreamWriter logWriter;

        public MainWindow()
        {
            InitializeComponent();

			initConfig();

			sensorDatas = new ObservableCollection<SensorData>();
			for(int i=0;i<config.Offsets.Length;i++)
			{
				sensorDatas.Add(new SensorData()
				{
					Offset = config.Offsets[i],
					Scale = config.Scales[i],
					LogEnabled = config.LogEnabled[i],
					Name = config.Names[i]
				});
			}
			sensorDataList.DataContext = sensorDatas;
			pathText.DataContext = config;

			portScanner = new AdvancedPortScanner(115200, 1024, 3);
			portScanner.OnFindPort += PortScanner_OnFindPort;
			portScanner.Start();

			remoteTime =  new DateTime(0);
			secTick = new Timer(500);
			secTick.Elapsed += SecTick_Elapsed;
			secTick.Start();

			isLogging = false;
        }

		private void SecTick_Elapsed(object sender, ElapsedEventArgs e)
		{
			localTime = DateTime.Now;
			Action a = () => { localTimeText.Text = localTime.ToString(); remoteTimeText.Text = remoteTime.ToString("HH:mm:ss"); };
			Dispatcher.BeginInvoke(a);
		}

		private void PortScanner_OnFindPort(AdvancedPortScanner sender, PortScannerEventArgs e)
		{
			Debug.WriteLine("[main] find port {0}", e.Link.Port.PortName);
			link = e.Link;
			link.OnReceivePackage += Link_OnReceivePackage;
			link.OpenPort();
			portScanner.Stop();
		}

		private void Link_OnReceivePackage(CommLink sender, LinkEventArgs e)
		{
			while(link.ReceivedPackageQueue.TryDequeue(out LinkPackage package))
			{
				analyzePackage((SBLinkPackage)package);
				//Debug.WriteLine("[Link] Recevie package size= {0} function={1} remain={2}", package.DataSize,package.Function,link.ReceivedPackageQueue.Count);
			}
			
		}

		void analyzePackage(SBLinkPackage package)
		{
			package.StartRead();
			remoteTime = package.Time;
			switch(package.Function)
			{
				case 1:
					for(int i=0;i<10;i++)
					{
						sensorDatas[i].Value = package.NextSingle();
					}
					for(int i=10;i<14;i++)
					{
						sensorDatas[i].Value = package.NextInt32();
					}
					if(isLogging)
					{
						logWriter.Write(package.Time.ToString("HH:mm:ss.fff"));
						for (int i = 0; i < 12; i++)
						{
							if (sensorDatas[i].LogEnabled)
							{								
								logWriter.Write("\t"+sensorDatas[i].ScaledValue);
							}
						}
						logWriter.Write(Environment.NewLine);
						logWriter.Flush();
					}
					break;
				default:

					break;
			}
		}

		void initConfig()
		{
			string path = Environment.CurrentDirectory;
			XmlSerializer xs = new XmlSerializer(typeof(Config));
			Stream s = new FileStream(path + "\\config.xml", FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.None);
			try
			{
				config = (Config)xs.Deserialize(s);
				s.Close();
			}
			catch
			{
				s.Close();
				s = new FileStream(path + "\\config.xml", FileMode.Create, FileAccess.ReadWrite, FileShare.None);
				config = Config.Default();
				xs.Serialize(s, config);
				s.Close();
				DirectoryInfo di = new DirectoryInfo(config.LogPath);
				if (!di.Exists)
					di.Create();
			}
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			for (int i = 0; i < config.Offsets.Length; i++)
			{
				config.Names[i] = sensorDatas[i].Name;
				config.Offsets[i] = sensorDatas[i].Offset;
				config.Scales[i] = sensorDatas[i].Scale;
				config.LogEnabled[i] = sensorDatas[i].LogEnabled;
			}
			XmlSerializer xs = new XmlSerializer(typeof(Config));
			Stream s = new FileStream(Environment.CurrentDirectory + "\\config.xml", FileMode.Create, FileAccess.Write, FileShare.None);
			xs.Serialize(s, config);
			s.Close();
		}

		private void PathBtn_Click(object sender, RoutedEventArgs e)
		{
			System.Windows.Forms.FolderBrowserDialog fbdig = new System.Windows.Forms.FolderBrowserDialog();
			var dr = fbdig.ShowDialog();
			if (dr != System.Windows.Forms.DialogResult.Cancel)
			{
				config.LogPath = fbdig.SelectedPath;
			}
		}

		private void TimeSync_Click(object sender, RoutedEventArgs e)
		{
			SBLinkPackage p = new SBLinkPackage();
			p.Function = 2;
			p.AddData((ushort)localTime.Millisecond);
			p.AddData((byte)localTime.Year);
			p.AddData((byte)localTime.Month);
			p.AddData((byte)localTime.Day);
			p.AddData((byte)localTime.Hour);
			p.AddData((byte)localTime.Minute);
			p.AddData((byte)localTime.Second);
			p.SetVerify();
			link.SendPackage(p);
		}

		private void LogBtn_Click(object sender, RoutedEventArgs e)
		{
			if(isLogging)
			{
				isLogging = false;
				logWriter.Close();
				logBtn.Content = "Start Log";
				
				sensorDataList.IsEnabled = true;
			}
			else
			{
				logBtn.Content = "Stop Log";				
				sensorDataList.IsEnabled = false;
				logWriter = new StreamWriter(config.LogPath + "\\" + DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss") + ".txt");
				logWriter.Write("Time");
				for (int i = 0; i < 12; i++)
				{
					if(sensorDatas[i].LogEnabled)
					{
						logWriter.Write("\t"+sensorDatas[i].Name);
					}
				}
				logWriter.Write(Environment.NewLine);
				logWriter.Flush();
				isLogging = true;
			}
		}
	}
}
