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

        protected override void OnClosed(EventArgs e)
        {
         
            base.OnClosed(e);
        }

        void OnRender(object sender, EventArgs e)
        {

        }

        protected override void OnActivated(EventArgs e)
        {
			//if (m_EditorSystem == null)
			//{
			//	m_EditorSystem = new EditorSystem();

			//	HwndSource hwnd = (HwndSource)HwndSource.FromVisual(RenderControl);
			//	EditorParameters parameters = new EditorParameters();
			//	parameters.windowHandle = hwnd.Handle;


			//	if (!m_EditorSystem.create(parameters))
			//	{
			//		m_EditorSystem = null;
			//		throw new NotSupportedException();
			//	}

			//	CompositionTarget.Rendering += OnRender;
            //}
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
			if (App.Editor != null)
			{
				App.Editor.onKeyDown(e);
			}
        }

        private void Window_KeyUp(object sender, KeyEventArgs e)
        {
			if (App.Editor != null)
			{
				App.Editor.onKeyUp(e);
			}
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
			if (App.Editor != null)
			{
				App.Editor.onMouseDown(e);
			}
        }

        private void Window_MouseUp(object sender, MouseButtonEventArgs e)
        {
			if (App.Editor != null)
			{
				App.Editor.onMouseUp(e);
			}
        }

        private void Window_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
			if (App.Editor != null)
			{
				App.Editor.onMouseDoubleClick(e);
			}
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
			if (App.Editor != null)
			{
				App.Editor.onMouseMove(e);
			}
        }

		private void DrawButton_Click(object sender, RoutedEventArgs e)
		{
			if (App.Editor != null)
			{
				App.Editor.render();
			}
		}
	}
}
