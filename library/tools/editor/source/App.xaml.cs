using System.Windows;
using tiki;

namespace editor.source
{
	/// <summary>
	/// Interaction logic for App.xaml
	/// </summary>
	public partial class App : Application
	{
		private static App s_instance = null;
		//private static EditorSystem s_editor = null;

		public static App Instance
		{
			get { return s_instance; }
		}

		//public static EditorSystem Editor
		//{
		//	get { return s_editor; }
		//}

		private void Application_Startup(object sender, StartupEventArgs e)
		{
			s_instance = this;
		}

		private void Application_Exit(object sender, ExitEventArgs e)
		{
			s_instance = null;
		}
	}
}
