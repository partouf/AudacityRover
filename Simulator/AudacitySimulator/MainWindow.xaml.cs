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

namespace AudacitySimulator
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private AudacityConnection Conn = null;

        public MainWindow()
        {
            InitializeComponent();
        }

        private AudacityConnection Connection()
        {
            if (Conn == null)
            {
                Conn = new AudacityConnection("127.0.0.1", 13666);
            }

            return Conn;
        }

        private void btnMoveForward_Click(object sender, RoutedEventArgs e)
        {
            Connection().Forward(10);
        }
        
        private void btnSystemInfoDirect_Click(object sender, RoutedEventArgs e)
        {
            Connection().InfoReport();
        }

        private void btnTakePicture_Click(object sender, RoutedEventArgs e)
        {
            Connection().CameraCapture();
        }

        private void btnStop_Click(object sender, RoutedEventArgs e)
        {
            Connection().Stop();
        }

        private void btnBackward_Click(object sender, RoutedEventArgs e)
        {
            Connection().Backward(10);
        }

        private void btnLeft_Click(object sender, RoutedEventArgs e)
        {
            Connection().Left(45);
        }

        private void btnRight_Click(object sender, RoutedEventArgs e)
        {
            Connection().Right(45);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Connection().Reboot();
        }
    }
}
