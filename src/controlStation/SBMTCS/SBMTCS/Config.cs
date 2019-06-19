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
		bool[] filterEnabled;
		double[] offsets;
		double[] scales;
		double[] lpf;
		string[] names;

		string logPath;

		double filterH;
		double filterL;
		int filterDepth;

		public event PropertyChangedEventHandler PropertyChanged;

		public bool[] LogEnabled { get => logEnabled; set => logEnabled = value; }
		public double[] Offsets { get => offsets; set => offsets = value; }
		public double[] Scales { get => scales; set => scales = value; }
		public string LogPath
		{
			get => logPath;
			set
			{
				logPath = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("LogPath"));
			}
		}

		public string[] Names { get => names; set => names = value; }
		public double FilterH
		{
			get => filterH;
			set
			{
				filterH = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("FilterH"));
			}
		}
		public double FilterL
		{
			get => filterL;
			set
			{
				filterL = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("FilterL"));
			}
		}

		public int FilterDepth
		{
			get => filterDepth;
			set
			{
				filterDepth = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("FilterDepth"));
			}
		}

		public bool[] FilterEnabled { get => filterEnabled; set => filterEnabled = value; }
		public double[] Lpf { get => lpf; set => lpf = value; }

		public static Config Default()
		{
			bool[] b = new bool[14];
			double[] o = new double[14];
			double[] s = new double[14];
			string[] n = new string[14];
			double[] l = new double[14];
			bool[] f = new bool[14];

			for(int i=0;i<14;i++)
			{
				b[i] = true;
				f[i] = i<10;
				l[i] = 0;
				o[i] = 0;
				s[i] = 1;
				n[i] = "Sensor " + i.ToString();
			}
			return new Config() { LogEnabled = b, FilterEnabled = f, Lpf = l, Scales = s, Offsets = o, LogPath = System.Environment.CurrentDirectory + "\\Logs", Names = n, FilterH = 0.03, FilterL = 0.03, FilterDepth = 2 };

		}
	}
}
