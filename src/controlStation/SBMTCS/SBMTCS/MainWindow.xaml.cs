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
using System.Diagnostics;

namespace SBMTCS
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
		AdvancedPortScanner portScanner;
		SerialLink link;

        public MainWindow()
        {
            InitializeComponent();

			portScanner = new AdvancedPortScanner(115200, 1024, 3);
			portScanner.OnFindPort += PortScanner_OnFindPort;
			portScanner.Start();
        }

		private void PortScanner_OnFindPort(AdvancedPortScanner sender, PortScannerEventArgs e)
		{
			Debug.WriteLine("[main] find port {0}", e.Link.Port.PortName);
			link = e.Link;
			link.OnReceivePackage += Link_OnReceivePackage;
			link.OpenPort();
		}

		private void Link_OnReceivePackage(CommLink sender, LinkEventArgs e)
		{
			while(link.ReceivedPackageQueue.TryDequeue(out LinkPackage package))
			{
				analyzePackage((SBLinkPackage)package);
				Debug.WriteLine("[Link] Recevie package size= {0}", package.DataSize);
			}
			
		}

		void analyzePackage(SBLinkPackage package)
		{

		}
	}
}
