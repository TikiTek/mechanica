using System.Windows.Controls;
using System.Windows.Threading;
using System;

namespace editor.source
{
	/// <summary>
	/// Interaction logic for RenderControl.xaml
	/// </summary>
	public partial class RenderControl : UserControl
	{
		private System.Windows.Forms.Control m_RenderTargetControl;
		private DispatcherTimer m_dispatcherTimer; 

		public RenderControl()
		{
			InitializeComponent();

			App.EditorReady += App_EditorReady;

			m_RenderTargetControl = new System.Windows.Forms.Control();
			m_RenderTargetControl.Dock = System.Windows.Forms.DockStyle.Fill;
			m_RenderTargetControl.SizeChanged += RenderTargetControl_SizeChanged;
			WinFormsHost.Child = m_RenderTargetControl;

			App.InitializeRenderer(m_RenderTargetControl.Handle);
		}

		void App_EditorReady(object sender, EventArgs e)
		{
			m_dispatcherTimer = new DispatcherTimer();
			m_dispatcherTimer.Tick += DispatcherTimer_Tick;
			m_dispatcherTimer.Interval = TimeSpan.FromMilliseconds(16.666);
			m_dispatcherTimer.Start();
		}

		void DispatcherTimer_Tick(object sender, EventArgs e)
		{
			App.Editor.update();
			App.Editor.render();

			m_RenderTargetControl.Invalidate();
		}

		void RenderTargetControl_SizeChanged(object sender, EventArgs e)
		{
			App.ResizeRenderer(m_RenderTargetControl.Size.Width, m_RenderTargetControl.Size.Height);
		}
	}
}
