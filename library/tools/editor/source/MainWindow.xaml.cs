using System.Diagnostics;
using System.Windows.Input;
using System.Windows;
using System;

namespace editor.source
{

	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();
					
            Debug.WriteLine("InitializeComponent");
		}
	}
}
