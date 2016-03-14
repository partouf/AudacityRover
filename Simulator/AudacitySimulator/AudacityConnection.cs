using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;

namespace AudacitySimulator
{
    enum module_t { modVoid = 0, modRemotePilot = 1, modAutoPilot = 2, modCamera = 3, modSystem = 9 };

    enum action_t
    {
        actVoid = 0,
        actSystemReboot = 1, actSystemRestartNetIF = 2, actSystemInfoReport = 3,
        actRemotePilotForward = 11, actRemotePilotBackward = 12, actRemotePilotLeft = 13, actRemotePilotRight = 14, actRemotePilotStop = 15,
        actCameraCapture = 21
    };

    class AudacityCommand
    {
        public UInt16 Module;
        public UInt16 Action;
        public UInt16 Param1;
        public UInt16 Param2;
        public string Param3;

        public AudacityCommand()
        {
            Module = 0;
            Action = 0;
            Param1 = 0;
            Param2 = 0;
            Param3 = "";
        }

        public byte[] ToBinaryCommandStr()
        {
            var Command = new byte[18+Param3.Length];

            Command[0] = (byte)'B';
            Command[1] = (byte)'I';
            Command[2] = (byte)'N';
            Command[3] = (byte)'C';
            Command[4] = (byte)'M';
            Command[5] = (byte)'D';

            UInt32 Len = (uint)Param3.Length + 12;
            Command[6] = (byte)((Len & 0xff000000) >> 24);
            Command[7] = (byte)((Len & 0x00ff0000) >> 16);
            Command[8] = (byte)((Len & 0x0000ff00) >> 8);
            Command[9] = (byte)((Len & 0x000000ff));

            Command[10] = (byte)((Module & 0xff00) >> 8);
            Command[11] = (byte)(Module & 0x00ff);

            Command[12] = (byte)((Action & 0xff00) >> 8);
            Command[13] = (byte)(Action & 0x00ff);

            Command[14] = (byte)((Param1 & 0xff00) >> 8);
            Command[15] = (byte)(Param1 & 0x00ff);

            Command[16] = (byte)((Param2 & 0xff00) >> 8);
            Command[17] = (byte)(Param2 & 0x00ff);

            var i = 0;
            foreach (var Ch in Param3)
            {
                Command[18 + i] = (byte)(Ch);
                i++;
            }

            return Command;
        }
    };

    class AudacityConnection
    {
        private string IP;
        private int Port;

        public AudacityConnection(string AIP, int APort)
        {
            IP = AIP;
            Port = APort;
        }

        private void SendCommand(AudacityCommand ACmd)
        {
            try
            {
                TcpClient client = new TcpClient(IP, Port);

                NetworkStream ns = client.GetStream();

                var cmdstr = ACmd.ToBinaryCommandStr();
                ns.Write(cmdstr, 0, cmdstr.Length);

                client.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        public void Forward(UInt16 AAmount)
        {
            var Cmd = new AudacityCommand();
            Cmd.Module = (UInt16)module_t.modRemotePilot;
            Cmd.Action = (UInt16)action_t.actRemotePilotForward;
            Cmd.Param1 = AAmount;

            SendCommand(Cmd);
        }

        public void Backward(UInt16 AAmount)
        {
            var Cmd = new AudacityCommand();
            Cmd.Module = (UInt16)module_t.modRemotePilot;
            Cmd.Action = (UInt16)action_t.actRemotePilotBackward;
            Cmd.Param1 = AAmount;

            SendCommand(Cmd);
        }

        public void Left(UInt16 ADegrees)
        {
            var Cmd = new AudacityCommand();
            Cmd.Module = (UInt16)module_t.modRemotePilot;
            Cmd.Action = (UInt16)action_t.actRemotePilotLeft;
            Cmd.Param1 = ADegrees;

            SendCommand(Cmd);
        }

        public void Right(UInt16 ADegrees)
        {
            var Cmd = new AudacityCommand();
            Cmd.Module = (UInt16)module_t.modRemotePilot;
            Cmd.Action = (UInt16)action_t.actRemotePilotRight;
            Cmd.Param1 = ADegrees;

            SendCommand(Cmd);
        }

        public void Stop()
        {
            var Cmd = new AudacityCommand();
            Cmd.Module = (UInt16)module_t.modRemotePilot;
            Cmd.Action = (UInt16)action_t.actRemotePilotStop;

            SendCommand(Cmd);
        }

        public void InfoReport()
        {
            var Cmd = new AudacityCommand();
            Cmd.Module = (UInt16)module_t.modSystem;
            Cmd.Action = (UInt16)action_t.actSystemInfoReport;

            SendCommand(Cmd);
        }

        public void CameraCapture()
        {
            var Cmd = new AudacityCommand();
            Cmd.Module = (UInt16)module_t.modCamera;
            Cmd.Action = (UInt16)action_t.actCameraCapture;

            SendCommand(Cmd);
        }

        public void Reboot()
        {
            var Cmd = new AudacityCommand();
            Cmd.Module = (UInt16)module_t.modSystem;
            Cmd.Action = (UInt16)action_t.actSystemReboot;

            SendCommand(Cmd);
        }
    }
}
