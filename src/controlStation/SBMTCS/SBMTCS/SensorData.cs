using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace SBMTCS
{
	public class SensorData : INotifyPropertyChanged
	{
		public event PropertyChangedEventHandler PropertyChanged;

		double value;
		double offset;
		double scale;
		bool logEnabled;
		double scaledValue;
		double lpfValue;
		double rawValue;

		bool filterEnabled;
		double lpf;
		int filterCounter;
		int dropoutCount;

		string name;

		Config config;

		public double Value
		{
			get => value;
			set
			{
				rawValue = value;
				if(!filterEnabled)
					this.value = value;
				else
				{
					if(value-this.value>config.FilterH||this.value-value>config.FilterL)
					{
                        if(name=="Sensor 4")
                        {

                        }
						filterCounter++;
						if(filterCounter>config.FilterDepth)
						{
							this.value = value;
							filterCounter = 0;
						}
                        else
                        {
                            DropoutCount++;
                        }
					}
					else
					{
						this.value = value;
						filterCounter = 0;
					}
				}
				scaledValue = (this.value + offset) * scale;
				LpfValue = lpf * LpfValue + (1 - lpf) * scaledValue;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("ScaledValue"));
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Value"));
			}

		}
		public double Offset
		{
			get => offset;
			set
			{
				offset = value;
				scaledValue = (this.value + offset) * scale;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("ScaledValue"));
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Offset"));
			}
		}
		public double Scale
		{
			get => scale;
			set
			{
				scale = value;
				scaledValue = (this.value + offset) * scale;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("ScaledValue"));
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Scale"));
			}
		}
		public bool LogEnabled
		{
			get => logEnabled;
			set
			{
				logEnabled = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("LogEnabled"));
			}
		}

		public double ScaledValue { get => scaledValue; private set => scaledValue = value; }
		public string Name
		{
			get => name;
			set
			{
				name = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Name"));
			}
		}

		public bool FilterEnabled
		{
			get => filterEnabled;
			set
			{
				filterEnabled = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("FilterEnabled"));
			}
		}
		public double Lpf
		{
			get => lpf;
			set
			{
				lpf = value;
				if (lpf < 1e-6)
					lpf = 0;
				if (lpf > 1)
					lpf = 1;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Lpf"));
			}
		}

		public int FilterCounter { get => filterCounter; set => filterCounter = value; }
		public double LpfValue { get => lpfValue; set => lpfValue = value; }
		public Config Config { get => config; set => config = value; }
		public double RawValue { get => rawValue; set => rawValue = value; }
		public int DropoutCount
		{
			get => dropoutCount;
			set
			{
				dropoutCount = value;
				PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("DropoutCount"));
			}
		}

		public SensorData()
		{
			value = 0;
			offset = 0;
			scale = 1;
			logEnabled = true;
			name = "Sensor";
		}
	}
}
