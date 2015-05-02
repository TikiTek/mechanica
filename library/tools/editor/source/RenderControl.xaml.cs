using System.Windows.Controls;
using System.Windows.Input;
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

			m_RenderTargetControl.MouseDown += RenderTargetControl_MouseDown;
			m_RenderTargetControl.MouseUp += RenderTargetControl_MouseUp;
			m_RenderTargetControl.MouseMove += RenderTargetControl_MouseMove;
			m_RenderTargetControl.MouseDoubleClick += RenderTargetControl_MouseDoubleClick;

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
		}

		void RenderTargetControl_SizeChanged(object sender, EventArgs e)
		{
			App.ResizeRenderer(m_RenderTargetControl.Size.Width, m_RenderTargetControl.Size.Height);
		}

		private void Control_KeyDown(object sender, KeyEventArgs e)
		{
			if (App.Editor != null)
			{
				App.Editor.Input.onKeyDown(e);
			}
		}

		private void Control_KeyUp(object sender, KeyEventArgs e)
		{
			if (App.Editor != null)
			{
				App.Editor.Input.onKeyUp(e);
			}
		}

		private void RenderTargetControl_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (App.Editor != null)
			{
				App.Editor.Input.onMouseDown(e);
			}
		}

		private void RenderTargetControl_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (App.Editor != null)
			{
				App.Editor.Input.onMouseUp(e);
			}
		}

		private void RenderTargetControl_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (App.Editor != null)
			{
				App.Editor.Input.onMouseMove(e);
			}
		}

		private void RenderTargetControl_MouseDoubleClick(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (App.Editor != null)
			{
				App.Editor.Input.onMouseDoubleClick(e);
			}
		}
	}
}
