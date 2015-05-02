using System;
using System.Windows;
using tiki;

namespace editor.source
{
	/// <summary>
	/// Interaction logic for App.xaml
	/// </summary>
	public partial class App : Application
	{
		public static event EventHandler EditorReady;

		private static App s_instance = null;
		private static EditorSystem s_editor = null;

		public static void InitializeRenderer(IntPtr handle)
		{
			if (s_editor != null)
			{
				return;
			}

			EditorParameters parameters;
			parameters.windowHandle = handle;

			s_editor = new EditorSystem();
			if (!s_editor.create(parameters))
			{
				MessageBox.Show("Unable to initialize EditorSystem.", "Editor", MessageBoxButton.OK, MessageBoxImage.Error);
				s_instance.Shutdown(1);
			}
			else if (EditorReady != null)
			{
				EditorReady(s_instance, new EventArgs());
			}
		}

		public static void ResizeRenderer(int width, int height)
		{
			if (!s_editor.resize(width, height))
			{
				MessageBox.Show("Unable to resize EditorSystem.", "Editor", MessageBoxButton.OK, MessageBoxImage.Error);
				s_instance.Shutdown(1);
			}
		}

		public static App Instance
		{
			get { return s_instance; }
		}

		public static EditorSystem Editor
		{
			get { return s_editor; }
		}

		private void Application_Startup(object sender, StartupEventArgs e)
		{
			s_instance = this;
		}

		private void Application_Exit(object sender, ExitEventArgs e)
		{
			if (s_editor != null)
			{
				s_editor.dispose();
				s_editor = null;
			}

			s_instance = null;
		}
	}
}
