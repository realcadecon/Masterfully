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
            process.StartInfo.FileName = path+"/Skeleton.exe";
            process.StartInfo.Arguments = "Warrior1";
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
            process.StartInfo.Arguments = "Warrior2";
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
            process.StartInfo.Arguments = "Lotus";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void ExtendedTriangleClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "ExtendedTriangle";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void LordOfDanceClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "LordOfDance";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void SideStretchClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "SideStretch";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void DownwardDogClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "DownwardDog";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }

        public void UpwardDogClick(object sender, RoutedEventArgs e)
        {
            // Run Skeleton.
            string path = Directory.GetCurrentDirectory();
            Process process = new Process();
            // Configure the process using the StartInfo properties.
            process.StartInfo.FileName = path + "/Skeleton.exe";
            process.StartInfo.Arguments = "UpwardDog";
            process.StartInfo.WindowStyle = ProcessWindowStyle.Maximized;
            process.Start();
            process.WaitForExit();// Waits here for the process to exit.
        }
    }
}
