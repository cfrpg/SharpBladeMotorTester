using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.ComponentModel;

namespace SBMTCS
{
	[Serializable]
	public class Config : INotifyPropertyChanged
	{
		bool[] logEnabled;
		double[] offsets;
		double[] scales;
		string[] names;

		string logPath;

		public event PropertyChangedEventHandler PropertyChanged;

		public bool[] LogEnabled { get => logEnabled; set => logEnabled = value; }
		public double[] Offsets { get => offsets; set => offsets = value; }
		public double[] Scales { get => scales; set => scales = value; }
		public string LogPath
		{
			get => logPath;
			set
			{
				logPath = value; PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("LogPath"));
			}
		}

		public string[] Names { get => names; set => names = value; }

		public static Config Default()
		{
			bool[] b = new bool[14];
			double[] o = new double[14];
			double[] s = new double[14];
			string[] n = new string[14];

			for(int i=0;i<14;i++)
			{
				b[i] = true;
				o[i] = 0;
				s[i] = 1;
				n[i] = "Sensor " + i.ToString();
			}
			return new Config() { LogEnabled = b, Scales = s, Offsets = o, LogPath = System.Environment.CurrentDirectory + "\\Logs", Names = n };

		}
	}
}
