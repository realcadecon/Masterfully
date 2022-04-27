using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
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

namespace Masterfully.MVVM.View
{
    /// <summary>
    /// Interaction logic for HomeView.xaml
    /// </summary>
    public partial class HomeView : UserControl
    {
        public HomeView()
        {
            InitializeComponent();
        }

        public void Warrior1Click(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 5";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void Warrior2Click(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 1";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void LotusClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 0";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void TriClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 2";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void LoDClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 3";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void DowndogClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 4";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void UpdogClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 6";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void ChairClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 8";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void Warrior3Click(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 9";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void ThunderClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 10";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void PlankClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 11";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        private void SideClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "-m 7";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }
    }
}
