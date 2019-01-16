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

namespace SampleClientGui
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            RegisterCallback();
        }

        private void RegisterCallback()
        {
            var eventBus = ((App)Application.Current).EventBus;
            eventBus.onUILogAddNewLine += OnLog;
        }
        private void dataGrid_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            
        }

        private void txtLog_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void button_login_Click(object sender, RoutedEventArgs e)
        {
            ((App)Application.Current).ConnetTrader();
        }

        private void button_reqOrderInsert_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button_qryOrder_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button_qryPosition_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button_qryFund_Click(object sender, RoutedEventArgs e)
        {

        }

        private void button_qryTrade_Click(object sender, RoutedEventArgs e)
        {

        }

        private void positionDataGrid_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        #region log 
        private delegate void ShowLogDelegate(UILogEventArgs e);
        public void OnLog(object sender, UILogEventArgs e)
        {
            if (!Dispatcher.CheckAccess())
            {
                Dispatcher.BeginInvoke(new ShowLogDelegate(DoShowLog), new object[] { e });
            }
            else
            {
                DoShowLog(e);
            }
        }

        private void DoShowLog(UILogEventArgs e)
        {
            if (txtLog.Text.Length > 10000)
                txtLog.Text = "";
            txtLog.AppendText(e.Line);
            if (IsVerticalScrollBarAtBottom)
            {
                this.txtLog.ScrollToEnd();
            }
        }

        public bool IsVerticalScrollBarAtBottom
        {
            get
            {
                bool atBottom = false;
                double dVer = this.txtLog.VerticalOffset;       //获取竖直滚动条滚动位置
                double dViewport = this.txtLog.ViewportHeight;  //获取竖直可滚动内容高度
                double dExtent = this.txtLog.ExtentHeight;      //获取可视区域的高度

                if (dVer + dViewport >= dExtent)
                {
                    atBottom = true;
                }
                else
                {
                    atBottom = false;
                }


                return atBottom;
            }
        }
        #endregion

        #region order callback
        #endregion
    }
}
