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

		string name;

		public double Value
		{
			get => value;
			set
			{
				this.value = value;
				scaledValue = (this.value + offset) * scale;
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
