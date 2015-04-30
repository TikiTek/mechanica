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
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

using tiki;

namespace editor.source
{

	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
        EditorSystem m_EditorSystem = null;


		public MainWindow()
		{
			InitializeComponent();

            System.Diagnostics.Debug.WriteLine("InitializeComponent");

		}

        protected override void OnClosed(EventArgs e)
        {
            if (m_EditorSystem != null)
            {
                m_EditorSystem.dispose();
            }
            
            base.OnClosed(e);
        }

        void OnRender(object sender, EventArgs e)
        {
            m_EditorSystem.update();

            m_EditorSystem.render();
        }

        protected override void OnActivated(EventArgs e)
        {

            if (m_EditorSystem == null)
            {
                m_EditorSystem = new EditorSystem();

                HwndSource hwnd = (HwndSource)HwndSource.FromVisual(RenderControl);
                EditorParameters parameters = new EditorParameters();
                parameters.windowHandle = hwnd.Handle;


                if (!m_EditorSystem.create(parameters))
                {
                    m_EditorSystem = null;
                    throw new NotSupportedException();
                }

                CompositionTarget.Rendering += OnRender;
            }
        }

#region Input Events
        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            m_EditorSystem.onKeyDown(e);
        }

        private void Window_KeyUp(object sender, KeyEventArgs e)
        {
            m_EditorSystem.onKeyUp(e);
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            m_EditorSystem.onMouseDown(e);
        }

        private void Window_MouseUp(object sender, MouseButtonEventArgs e)
        {
            m_EditorSystem.onMouseUp(e);
        }

        private void Window_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            m_EditorSystem.onMouseDoubleClick(e);
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
            m_EditorSystem.onMouseMove(e);
        }

#endregion



        

	}
}
