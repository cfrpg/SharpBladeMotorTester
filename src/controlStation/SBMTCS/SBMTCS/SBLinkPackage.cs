using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SBMTCS
{
    public class SBLinkPackage : LinkPackage
    {
        DateTime time;

        public DateTime Time { get => time; set => time = value; }

        public override int HeaderSize { get => 8; }

        public override LinkProtocol Protocol { get => LinkProtocol.SBLink; }

        public override int PackageSize { get => dataSize + HeaderSize + 1; }
               
        public SBLinkPackage():base(280)
        {

        }

        public override PackageParseResult ReadFromBuffer(byte[] buff, int length, int offset)
        {
			return checkBuffer(buff, length, offset);
        }

		public override PackageParseResult ReadFromBuffer(byte[] buff, int length, int offset, out int dataUsed)
		{
			var r= checkBuffer(buff, length, offset);
			dataUsed = DataSize + HeaderSize + 1;
			return r;
		}

		private PackageParseResult checkBuffer(byte[] buff, int length, int offset)
		{
			if (length - offset < HeaderSize)
				return PackageParseResult.NoEnoughData;
			if (!(buff[offset + 0] == 0xFC))
				return PackageParseResult.NoSTX;
			int len = buff[offset + 1];
			if (len + HeaderSize + 1 + offset > length)
				return PackageParseResult.NoEnoughData;
			byte sum = 0;
			for (int i = 0; i < len + HeaderSize; i++)
			{
				sum ^= buff[offset + i];
			}
			if (buff[offset + len + HeaderSize] != sum)
				return PackageParseResult.BadCheckSum;
			for (int i = 0; i < buffer.Length; i++)
			{
				buffer[i] = 0;
			}
			for (int i = 0; i < len + HeaderSize + 1; i++)
			{
				buffer[i] = buff[offset + i];
			}
			function = buffer[2];
			dataSize = len;

			return PackageParseResult.Yes;

		}

		public override void SetVerify()
        {
            base.SetVerify();
        }

		public override LinkPackage Clone()
		{
			SBLinkPackage package = new SBLinkPackage();
			buffer.CopyTo(package.buffer, 0);
			package.dataSize = dataSize;
			package.function = function;
			package.time = time;
			package.timeStamp = timeStamp;
			return package;
		}
	}
}
