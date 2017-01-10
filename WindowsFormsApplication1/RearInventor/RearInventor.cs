using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace RearInventor
{
    public partial class RearForm : Form
    {
        #region "常量"

        /// <summary>
        /// 最大属性值
        /// </summary>
        public const int MAX_POINT = 1200;
        /// <summary>
        /// 最大疲劳值
        /// </summary>
        public const int MAX_TIRED = 300;
        /// <summary>
        /// 非天书洗点数
        /// </summary>
        public const int REDUCE_POINT = 3;
        /// <summary>
        /// 最大行数
        /// </summary>
        public const int MAX_ROWCOUNT = 12 * 12 * 3;// 年*月*(3次/月）
        /// <summary>
        /// 以防高级学习造成的点数浪费设置的定量(演算用)
        /// </summary>
        public const int POINT_LEFT = 30;

        /// <summary>
        /// 属性点说明文本
        /// </summary>
        private string[] pointComment = { "气质", "耐力", "智力", "内力", "名气", "叛逆", "道德", "玩性" };

        #region "行动组"

        /// <summary>
        /// 行动方式{ "工作", "学习", "生活" }
        /// </summary>
        private string[] actionGroup = { "工作", "学习", "生活" };
        /// <summary>
        /// 工作
        /// </summary>
        private string[] work = { "家务", "伴读", "火工", "水手", "收银", "煎药", "引路", "小贩", "打手", "卖萌", "跑堂" };
        /// <summary>
        /// 学习
        /// </summary>
        private string[] learn = { "音律", "美术", "诗文", "武术", "舞蹈", "建筑", "宗教", "骑术" };
        /// <summary>
        /// 生活
        /// </summary>
        private string[] live = { "国子监", "休息", "皇宫", "海边", "野外", "太医院", "地府", "黑市", "赌场", "旅游", "青楼" };

        #endregion

        #endregion

        #region "变量"

        /// <summary>
        /// 0疲劳警告
        /// </summary>
        bool zerotired = false;

        /// <summary>
        /// 属性点{气质,耐力,智力,内力,名气,道德,叛逆,玩性}
        /// </summary>
        private int[] point = { 36, 36, 36, 36, 36, 36, 0, 0};
        
        /// <summary>
        /// 当前的属性点
        /// </summary>
        private int[] currentPoint = new int[8];

        /// <summary>
        /// 资源{疲劳,养育金}
        /// </summary>
        private int[] resource = { 0, 9800 };

        /// <summary>
        /// 当期资源
        /// </summary>
        private int[] currentResource = new int[2];

        /// <summary>
        /// 装备点数{气质,耐力,智力,内力,名气,道德,叛逆}
        /// </summary>
        private int[] equipPoint = new int[7];

        /// <summary>
        /// 起始月份
        /// </summary>
        private int startMonth = 0;

        /// <summary>
        /// 起始点数
        /// </summary>
        private int[] startPoint = new int[8];

        /// <summary>
        /// 起始资源
        /// </summary>
        private int[] startResource = new int[2];

        #region "行动统计"

        /// <summary>
        /// 工作次数
        /// </summary>
        private int[] workCount = null;
        /// <summary>
        /// 学习次数
        /// </summary>
        private int[] learnCount = null;
        /// <summary>
        /// 生活次数
        /// </summary>
        private int[] liveCount = null;

        #endregion

        /// <summary>
        /// 文件读写类
        /// </summary>
        private FileHelper fileHelper = null;

        #endregion

        public RearForm()
        {
            InitializeComponent();
        }

        #region "事件"

        /// <summary>
        /// 画面load事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RearInventor_Load(object sender, EventArgs e)
        {
            this.dgvRear.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            // 行动的下拉框内容设置
            for (int i = 0; i < actionGroup.Length; i++)
            {
                this.cboAction.Items.Add(actionGroup[i]);
            }
            // 初始化
            initial();
        }

        /// <summary>
        /// DataGridView行增加事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void dgvRear_RowsAdded(object sender, DataGridViewRowsAddedEventArgs e)
        {
            int RowIndex = e.RowIndex;

            this.dgvRear.Rows[RowIndex].Cells[0].Value
                = (getAge(RowIndex)) + "岁" + ((RowIndex) % 36 / 3 + "个月");
        }

        /// <summary>
        /// 画面关闭事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RearInventor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult.OK == MessageBox.Show(this, "close", "sure",
                MessageBoxButtons.OKCancel, MessageBoxIcon.Question))
            {
                e.Cancel = false;
            }
            else {
                e.Cancel = true;
            }
        }

        /// <summary>
        /// 重置按钮
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnReset_Click(object sender, EventArgs e)
        {
            initial();
            // 显示到最后一行
            this.dgvRear.FirstDisplayedScrollingRowIndex = this.dgvRear.RowCount - 1;
            // 选择最后一行
            this.dgvRear.Rows[this.dgvRear.RowCount - 1].Selected = true;
        }

        /// <summary>
        /// 下一次行动按钮
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnnext_Click(object sender, EventArgs e)
        {
            if (this.cboDetail.SelectedIndex < 0)
            {
                // 基于“返回到选择行的数据”以及“导入”按钮按下后的操作考虑，当具体行动内容不是空的情况下执行
                if (this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[10].Value != null
                    && !"".Equals(this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[10].Value.ToString()))
                {
                    for (int i = 0; i < currentPoint.Length - 1; i++)
                    {
                        // 表格中的数据减去装备上的点数
                        currentPoint[i] = parseValue(this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[i + 1].Value.ToString())
                                            - equipPoint[i];
                    }
                    // 装备没有玩性
                    currentPoint[currentPoint.Length - 1] = parseValue(this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[8].Value.ToString());
                    currentResource[0] = parseValue(this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[12].Value.ToString());
                    currentResource[1] = parseValue(this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[13].Value.ToString());
                    // 保存数据
                    dataCopy(false);
                    // 行增加，进行下一次的行动
                    this.dgvRear.Rows.Add();
                    // 显示最后一行，为了滚动条的控制
                    this.dgvRear.FirstDisplayedScrollingRowIndex = this.dgvRear.RowCount - 1;
                    // 解锁行动选项
                    this.cboAction.Enabled = true;
                }
                return;
            }

            if (resource[0] >= MAX_TIRED && this.cboAction.Text != actionGroup[2])
            {
                MessageBox.Show("疲劳值超过300不能进行学习和工作");
                return;
            }

            if ((resource[1] < 120 && (this.cboAction.Text == actionGroup[1] || this.cboDetail.Text == live[2]))
                || (resource[1] < 360 && this.chkPay.Checked && this.chkPay.Enabled)
                )
            {
                MessageBox.Show("养育金不足，请工作赚钱。");
                return;
            }

            // TODO：学习的控制靠用户自己选择
            //if (false)
            //{
            //    if (DialogResult.Cancel == MessageBox.Show(this, "低级学习已经30次了，确定要继续？", "确认",
            //                   MessageBoxButtons.OKCancel, MessageBoxIcon.Question))
            //    {
            //        return;
            //    }
            //}

            DataGridViewTextBoxCell cell1 = (DataGridViewTextBoxCell)this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[1];
            if (cell1.Value == null || cell1.Value.ToString() == "")
            {
                opeartion();
            }
            // 0疲劳警告
            if (zerotired)
            {
                if (DialogResult.Cancel == MessageBox.Show(this,
                                                        "此次行动减少疲劳值过大，确定要继续？",
                                                        "确认",
                                                        MessageBoxButtons.OKCancel,
                                                        MessageBoxIcon.Question))
                {
                    return;
                }
            }
            // 行动统计
            actionCount(this.dgvRear.RowCount - 1);
            // 显示行动统计结果
            displayCount();
            if (this.dgvRear.RowCount == MAX_ROWCOUNT)
            {
                MessageBox.Show("孩子已经成年！");
                //TODO: 似乎多余的功能
                //// 获取成年的数据
                //int[] calPoint = new int[7];
                //for (int i = 0; i < calPoint.Length; i++)
                //{
                //    calPoint[i] = parseValue(this.dgvRear.Rows[MAX_ROWCOUNT - 1].Cells[i + 1].Value.ToString());
                //}
                //int wxPoint = parseValue(this.dgvRear.Rows[MAX_ROWCOUNT - 1].Cells[8].Value.ToString());

                //if (wxPoint > 0)
                //{
                //    if (DialogResult.OK == MessageBox.Show(this,
                //                                                "你还有没有洗除的玩性，清除吗？",
                //                                                "确认",
                //                                                MessageBoxButtons.OKCancel,
                //                                                MessageBoxIcon.Question))
                //    {
                //        return;
                //    }
                //}

                //MessageBox.Show("评价 = " + Math.Floor(calc(calPoint, wxPoint)).ToString());

                return;
            }
            // 当前的行动数据保存
            dataCopy(false);
            this.dgvRear.Rows.Add();
            // 控制滚动条，显示表格最下边行
            this.dgvRear.FirstDisplayedScrollingRowIndex = this.dgvRear.RowCount - 1;
        }

        /// <summary>
        /// 行动方式选择改变事件：加载具体行动方式下拉框内容
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cbxAction_SelectedValueChanged(object sender, EventArgs e)
        {
            this.chkPay.Enabled = false;
            // 当前行的年龄
            int currentAge = getAge(this.dgvRear.RowCount - 1);
            // 行动方式：工作
            if (this.cboAction.Text == actionGroup[0])
            {
                this.cboDetail.Items.Clear();
                int workItemCount = detailDropControl(currentAge, actionGroup[0]);
                for (int i = 0; i < workItemCount; i++)
                {
                    this.cboDetail.Items.Add(work[i]);
                }
            }
            // 行动方式：学习
            else if (this.cboAction.Text == actionGroup[1])
            {
                this.cboDetail.Items.Clear();
                // 学习内容全加载
                foreach (string learnitem in learn)
                {
                    this.cboDetail.Items.Add(learnitem);
                }
            }
            // 行动方式：生活
            else if (this.cboAction.Text == actionGroup[2])
            {
                this.cboDetail.Items.Clear();
                int liveItemCount = detailDropControl(currentAge, actionGroup[2]);
                for (int i = 0; i < liveItemCount; i++)
                {
                    this.cboDetail.Items.Add(live[i]);
                }
            }

        }

        /// <summary>
        /// 具体行动下拉框内容改变事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cbxDetail_SelectedValueChanged(object sender, EventArgs e)
        {
            // 给乞丐钱复选框的可用与否控制
            if (actionGroup[2] == this.cboAction.Text
                &&(live[3] == this.cboDetail.Text
                || live[4] == this.cboDetail.Text
                || live[7] == this.cboDetail.Text
                || live[8] == this.cboDetail.Text
                || live[10] == this.cboDetail.Text))
            {
                this.chkPay.Enabled = true;
            }
            else
            {
                this.chkPay.Enabled = false;
            }
            // 改变行数据
            opeartion();
        }

        /// <summary>
        /// 给乞丐钱 复选框事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void chbPay_CheckedChanged(object sender, EventArgs e)
        {
            DataGridViewTextBoxCell cell11 = (DataGridViewTextBoxCell)this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[11];

            if (this.chkPay.Checked && this.chkPay.Enabled)
            {
                cell11.Value = "给";
                dataCopy(true);
                valueControl(this.cboAction.Text, this.cboDetail.Text);
                setCellValue(this.dgvRear.RowCount - 1);
            }
            else
            {
                cell11.Value = "";
                dataCopy(true);
                valueControl(this.cboAction.Text, this.cboDetail.Text);
                setCellValue(this.dgvRear.RowCount - 1);
            }
        }

        /// <summary>
        /// 职业评价的“计算”按钮按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnCal_Click(object sender, EventArgs e)
        {
            int[] calPoint = new int[7];
            int wxPoint = 0;
            // 使用表格中数据时
            if (this.chktable.Checked && this.dgvRear.SelectedRows.Count != 0)
            {
                if (this.dgvRear.SelectedRows[0].Cells[1].Value != null
                    && !"".Equals(this.dgvRear.SelectedRows[0].Cells[1].Value.ToString()))
                {
                    for (int i = 0; i < calPoint.Length; i++)
                    {
                        calPoint[i] = parseValue(this.dgvRear.SelectedRows[0].Cells[i + 1].Value.ToString());
                    }
                    wxPoint = parseValue(this.dgvRear.SelectedRows[0].Cells[8].Value.ToString());
                }
                else
                {
                    this.lblevaluate.Text = "";
                    MessageBox.Show("所选择行没有数据，请重新选择");
                    return;
                }
            }
            else
            {
                calPoint[0] = parseValue(this.tbxpqiz.Text);
                calPoint[1] = parseValue(this.tbxpnal.Text);
                calPoint[2] = parseValue(this.tbxpzl.Text);
                calPoint[3] = parseValue(this.tbxpnl.Text);
                calPoint[4] = parseValue(this.tbxpmq.Text);
                calPoint[5] = parseValue(this.tbxppn.Text);
                calPoint[6] = parseValue(this.tbxpdd.Text);
                for (int i = 0; i < calPoint.Length; i++)
                {
                    if (calPoint[i] > MAX_POINT)
                    {
                        calPoint[i] = MAX_POINT;
                    }
                }
                wxPoint = parseValue(this.tbxpwx.Text);
            }
            //this.lblevaluate.Text = calc(calPoint).ToString().Split(char.Parse("."))[0];
            // 显示职业评价计算结果double值的整数部分——非四舍五入
            this.lblevaluate.Text = Math.Floor(calc(calPoint, wxPoint)).ToString();
        }

        /// <summary>
        /// 减少玩性的按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnReduce_Click(object sender, EventArgs e)
        {
            if (this.dgvRear.RowCount < 1)
            {
                return;
            }
            // “玩性”单元格
            DataGridViewTextBoxCell cell8 = (DataGridViewTextBoxCell)this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[8];
            // 没有数据时不执行
            if (cell8.Value == null || "".Equals(cell8.Value.ToString()) || "0".Equals(cell8.Value.ToString()))
            {
                return;
            }
            // 非天书洗点（板载师贡以及类似【-3玩性，加到最高属性上】的方式）
            if (this.rdbWay2.Checked)
            {
                int[] tempPoint = new int[currentPoint.Length - 1];
                for (int i = 0; i < currentPoint.Length - 1; i++)
                {
                    tempPoint[i] = currentPoint[i];
                }
                // 排序，以知其最大属性值
                Array.Sort(tempPoint);
                if (tempPoint[tempPoint.Length - 1] > MAX_POINT - REDUCE_POINT)
                {
                    if (DialogResult.Cancel == MessageBox.Show(this,
                                                "最高属性已经达到" + tempPoint[tempPoint.Length - 1]
                                                    + ",不建议通过此途径减少玩性。\r\n仍然要这么通过此途径减去玩性吗？",
                                                "确认",
                                                MessageBoxButtons.OKCancel,
                                                MessageBoxIcon.Question))
                    {
                        return;
                    }
                }
                // 是否已增加过玩性变量，防止2属性并列最大时重复加值
                bool increased = false;
                for (int i = 0; i < currentPoint.Length - 1; i++)
                {
                    if (currentPoint[i] == tempPoint[tempPoint.Length - 1] && !increased)
                    {
                        currentPoint[7] -= REDUCE_POINT;
                        int tempP = 0;
                        // 没有多余的玩性可供减少时
                        if (currentPoint[7] < 0)
                        {
                            tempP = currentPoint[7];
                            currentPoint[7] = 0;
                        }
                        currentPoint[i] += REDUCE_POINT + tempP;
                        if (currentPoint[i] > MAX_POINT)
                        {
                            currentPoint[i] = MAX_POINT;
                        }
                        increased = true;
                    }
                }
            }
            // 天书洗点（即自由洗点）
            else if (this.rdbWay1.Checked)
            {
                int[] calPoint = new int[7];
                calPoint[0] = parseValue(this.tbxrqiz.Text);
                calPoint[1] = parseValue(this.tbxrnal.Text);
                calPoint[2] = parseValue(this.tbxrzl.Text);
                calPoint[3] = parseValue(this.tbxrnl.Text);
                calPoint[4] = parseValue(this.tbxrmq.Text);
                calPoint[5] = parseValue(this.tbxrpn.Text);
                calPoint[6] = parseValue(this.tbxrdd.Text);
                int sum = 0;
                for (int i = 0; i < calPoint.Length; i++)
                {
                    sum += calPoint[i];
                }
                if (sum > currentPoint[currentPoint.Length - 1])
                {
                    MessageBox.Show("没有足够的玩性点数可以洗，请检查。");
                    return;
                }
                currentPoint[currentPoint.Length - 1] -= sum;
                for (int i = 0; i < calPoint.Length; i++)
                {
                    currentPoint[i] += calPoint[i];
                    if (currentPoint[i] > MAX_POINT)
                    {
                        MessageBox.Show("洗的玩性使得" + pointComment[i] + "超出上限造成属性点浪费，请注意。");
                        currentPoint[i] = MAX_POINT;
                    }
                }
            }
            // 设置洗点后属性数据到表格中
            setCellValue(this.dgvRear.RowCount - 1);
        }

        /// <summary>
        /// 玩性洗点方式单选按钮改变事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void rdbWay1_CheckedChanged(object sender, EventArgs e)
        {
            if (this.rdbWay1.Checked)
            {
                this.tbxrqiz.Enabled = true;
                this.tbxrnal.Enabled = true;
                this.tbxrzl.Enabled = true;
                this.tbxrnl.Enabled = true;
                this.tbxrmq.Enabled = true;
                this.tbxrpn.Enabled = true;
                this.tbxrdd.Enabled = true;
            }
            else
            {
                this.tbxrqiz.Enabled = false;
                this.tbxrnal.Enabled = false;
                this.tbxrzl.Enabled = false;
                this.tbxrnl.Enabled = false;
                this.tbxrmq.Enabled = false;
                this.tbxrpn.Enabled = false;
                this.tbxrdd.Enabled = false;
            }
        }

        /// <summary>
        /// 返回到选择行按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnReturn_Click(object sender, EventArgs e)
        {
            // TODO: 由于装备点数与1200的属性上限要求，可能会导致计算值偏小
            if (this.dgvRear.SelectedRows.Count != 0)
            {
                if (this.dgvRear.SelectedRows[0].Cells[1].Value != null
                    && !"".Equals(this.dgvRear.SelectedRows[0].Cells[1].Value.ToString()))
                {
                    if (DialogResult.Cancel == MessageBox.Show(this,
                                                              "此操作会删除选择行后的数据，并可能导致点数计算不正："
                                                              + "\r\n"
                                                              + "如装备上耐力点数80，实际点数1150，表格中显示1200（达到上限），"
                                                              + "\r\n"
                                                              + "此次操作会将实际点数设置为1120",
                                                              "操作确认",
                                                              MessageBoxButtons.OKCancel,
                                                              MessageBoxIcon.Question))
                    {
                        return;
                    }
                    // 清空具体行动下拉框（基于如6岁不能去赌场此类的考虑）
                    this.cboDetail.Items.Clear();
                    // 行动与具体行动为空，以表格中为准，不可更改
                    this.cboDetail.SelectedItem = null;
                    this.cboAction.SelectedItem = null;
                    // 锁定行动下拉框，不可选状态
                    this.cboAction.Enabled = false;
                }
                else
                {
                    this.lblevaluate.Text = "";
                    MessageBox.Show("所选择行没有数据，请选择");
                    return;
                }
            }
            // 选择行之后的数据全删除
            for (int i = this.dgvRear.RowCount - 1; i > this.dgvRear.SelectedRows[0].Index; i--)
            {
                this.dgvRear.Rows.RemoveAt(i);
            }
            // 0疲劳警告初始化
            zerotired = false;
            // 行动次数初始化
            workCount = new int[work.Length];
            learnCount = new int[learn.Length];
            liveCount = new int[live.Length + 1];// +1 for beg
            // 行动次数计算
            for (int i = 0; i < this.dgvRear.RowCount; i++)
            {
                actionCount(i);
            }
            displayCount();
            // 显示到最后一行
            this.dgvRear.FirstDisplayedScrollingRowIndex = this.dgvRear.RowCount - 1;
        }

        #region "导入导出"

        /// <summary>
        /// 导入按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnInput_Click(object sender, EventArgs e)
        {
            // 用户选择了文件时
            if (DialogResult.OK == ofdFilePath.ShowDialog())
            {
                // 得到用户指定的文件路径
                string filePath = ofdFilePath.FileName;
                if (fileHelper == null)
                {
                    fileHelper = new FileHelper();
                }
                ArrayList fileContent = fileHelper.ImportFile(filePath);
                // 文件读取成功时
                if (fileContent != null)
                {
                    // 起始月份获得
                    startMonth = parseValue(fileContent[1].ToString());

                    // 起始点数获得
                    string[] stPointValue = fileContent[3].ToString().Split(char.Parse(FileHelper.separative));
                    int i = 0;
                    foreach (string stp in stPointValue)
                    {
                        if (i >= startPoint.Length)
                        {
                            MessageBox.Show("请确认文件是否有误。");
                            return;
                        }
                        startPoint[i] = parseValue(stp);
                        i++;
                    }

                    // 起始资源获得
                    string[] stResourceValue = fileContent[5].ToString().Split(char.Parse(FileHelper.separative));
                    i = 0;
                    foreach (string stp in stResourceValue)
                    {
                        if (i >= startResource.Length)
                        {
                            MessageBox.Show("请确认文件是否有误。");
                            return;
                        }
                        startResource[i] = parseValue(stp);
                        i++;
                    }

                    // 装备点数获得
                    string[] equipPointValue = fileContent[7].ToString().Split(char.Parse(FileHelper.separative));
                    i = 0;
                    foreach (string stp in equipPointValue)
                    {
                        if (i >= equipPoint.Length)
                        {
                            MessageBox.Show("请确认文件是否有误。");
                            return;
                        }
                        equipPoint[i] = parseValue(stp);
                        i++;
                    }

                    // 表格外的内容（包括标题头）删除
                    for (i = 8; i >= 0; i--)
                    {
                        fileContent.RemoveAt(i);
                    }

                    // 清空表数据
                    this.dgvRear.Rows.Clear();
                    // 加载表格内的数据
                    foreach(object item in fileContent)
                    {
                        this.dgvRear.Rows.Add();
                        string[] content = item.ToString().Split(char.Parse(FileHelper.separative));
                        i = 0;
                        foreach(string it in content)
                        {
                            if (i >= this.dgvRear.ColumnCount)
                            {
                                MessageBox.Show("请确认文件是否有误。");
                                return;
                            }
                            this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[i].Value = it;
                            i++;
                        }
                    }
                    // 显示到最后一行
                    this.dgvRear.FirstDisplayedScrollingRowIndex = this.dgvRear.RowCount - 1;
                    // 选择最后一行
                    this.dgvRear.Rows[this.dgvRear.RowCount - 1].Selected = true;
                    // 清空具体行动下拉框（基于如6岁不能去赌场此类的考虑）
                    this.cboDetail.Items.Clear();
                    // 行动与具体行动为空，以表格中为准，不可更改
                    this.cboDetail.SelectedItem = null;
                    this.cboAction.SelectedItem = null;
                    // 锁定行动下拉框，不可选状态
                    this.cboAction.Enabled = false;

                    // 显示起始数据
                    // 初始距离6岁的月份数
                    this.tbxmonth.Text = startMonth.ToString();
                    // 初始点数
                    this.tbxqiz.Text = startPoint[0].ToString();
                    this.tbxnal.Text = startPoint[1].ToString();
                    this.tbxzl.Text = startPoint[2].ToString();
                    this.tbxnl.Text = startPoint[3].ToString();
                    this.tbxmq.Text = startPoint[4].ToString();
                    this.tbxpn.Text = startPoint[5].ToString();
                    this.tbxdd.Text = startPoint[6].ToString();
                    this.tbxwx.Text = startPoint[7].ToString();
                    // 装备点数
                    this.tbxepqiz.Text = equipPoint[0].ToString();
                    this.tbxepnal.Text = equipPoint[1].ToString();
                    this.tbxepzl.Text = equipPoint[2].ToString();
                    this.tbxepnl.Text = equipPoint[3].ToString();
                    this.tbxepmq.Text = equipPoint[4].ToString();
                    this.tbxeppn.Text = equipPoint[5].ToString();
                    this.tbxepdd.Text = equipPoint[6].ToString();
                }
                else
                {
                    MessageBox.Show("打开文件出错，请确认文件是否有误。");
                }
            }
        }

        /// <summary>
        /// 导出按钮事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnOutput_Click(object sender, EventArgs e)
        {
            if (this.dgvRear.RowCount < 1)
            {
                return;
            }
            else if(this.dgvRear.Rows[this.dgvRear.RowCount - 1].Cells[10].Value == null)
            {
                MessageBox.Show("表格最后一行没有数据，请进行具体行动选择。");
                return;
            }
            string path = @"D:\RearInventor.txt";
            if (fileHelper == null)
            {
                fileHelper = new FileHelper();
            }
            // 导出文件，成功的场合打开文件，失败则给出提示
            if (fileHelper.ExportToFile(this.dgvRear, path, startMonth, startPoint, startResource, equipPoint))
            {                
                if (DialogResult.OK == MessageBox.Show(this,
                                                        "文件导出成功，文件目录：\r\n" 
                                                        + path
                                                        + "需要打开吗？",
                                                        "文件打开确认",
                                                        MessageBoxButtons.OKCancel,
                                                        MessageBoxIcon.Question))
                {
                    System.Diagnostics.Process.Start(path);
                }
            }
            else
            {
                MessageBox.Show("文件导出失败，请确认" + path +"文件没有被打开,或者" 
                                + path.Substring(0,2) + "磁盘已经满了"); 
            }

        }

        #region "演算按钮事件"

        /// <summary>
        /// 演算按钮事件
        /// </summary>
        /// <remarks>
        /// 根据用户给的初始属性、资源以及目标属性，进行行动过程的演算
        /// </remarks>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnTry_Click(object sender, EventArgs e)
        {
            if (DialogResult.Cancel == MessageBox.Show(this,
                                                       "此操作会清空表格内容，如果没保存请取消此操作，先导出数据。"
                                                        + "\r\n演算过程可能需要你等待一段时间。"
                                                        + "\r\n计算结果可能消耗偏大（多几次卖萌、旅游）。"
                                                        + "\r\n请善于利用属性点下限为0，手动模拟养育过程。",
                                                       "提示",
                                                       MessageBoxButtons.OKCancel,
                                                       MessageBoxIcon.Question))
            {
                return;
            }

            // 执行重置按钮事件
            this.btnReset_Click(this.btnReset, new EventArgs());
            // 已经成年情况下返回
            if (this.dgvRear.RowCount == MAX_ROWCOUNT)
            {
                return;
            }
            // 演算是否出错变量
            bool error = false;
            // 是否需要花钱
            bool moneyCost = false;
            // 目标属性
            int[] targetPoint = new int[7];
            targetPoint[0] = parseValue(this.tbxpqiz.Text);
            targetPoint[1] = parseValue(this.tbxpnal.Text);
            targetPoint[2] = parseValue(this.tbxpzl.Text);
            targetPoint[3] = parseValue(this.tbxpnl.Text);
            targetPoint[4] = parseValue(this.tbxpmq.Text);
            targetPoint[5] = parseValue(this.tbxppn.Text);
            targetPoint[6] = parseValue(this.tbxpdd.Text);
            // 设置了目标属性时候
            for (int i = 0; i < targetPoint.Length; i++)
            {
                // 目标属性太高（超过1200），设置为1200
                if (targetPoint[i] > MAX_POINT)
                {
                    targetPoint[i] = MAX_POINT;
                }
            }
            // 用户设置目标属性与否变量
            bool noSet = true;
            // 未设置目标属性时候
            for (int i = 0; i < targetPoint.Length; i++)
            {
                // 目标属性太高（超过1200），设置为1200
                if (targetPoint[i] != 0)
                {
                    noSet = false;
                    break;
                }
            }
            if (noSet)
            {
                // 优化：5000评价 = 名气1200耐力1200内力1100气质1100智力1100叛逆900道德250
                targetPoint[4] = MAX_POINT;
                targetPoint[1] = MAX_POINT;
                targetPoint[3] = 1100;
                targetPoint[0] = 1100;
                targetPoint[2] = 1100;
                targetPoint[5] = 900;
                targetPoint[6] = 250;
            }

            // 4大学习属性点的临时变量
            int[] tempP = new int[4];


            // 从当前行开始计算
            for (int i = this.dgvRear.RowCount - 1; i < MAX_ROWCOUNT; i++)
            {
                if (error)
                {
                    MessageBox.Show("演算出错\r\n请检查输入内容是否有误\r\n或者自己手动选择下一步的行动");
                    return;
                }
                // 当前年龄获取
                int currentAge = getAge(this.dgvRear.RowCount - 1);

                #region "计算是否需要卖萌旅游"

                if (currentAge >= 13)
                {
                    // 计算点数和装备点数和
                    tempP[0] = point[0] + equipPoint[0];
                    tempP[1] = point[1] + equipPoint[1];
                    tempP[2] = point[2] + equipPoint[2];
                    tempP[3] = point[3] + equipPoint[3];
                    // 超过部分设置为属性上限（1200）
                    for (int j = 0; j < 4; j++)
                    {
                        if (tempP[j] > MAX_POINT)
                        {
                            tempP[j] = MAX_POINT;
                        }
                    }
                    // 计算4大学习属性的剩余点数和(考虑上装备的属性，另玩性可以洗除到4大学习属性点上)
                    int sumPoint = targetPoint[0] + targetPoint[1] + targetPoint[3] + targetPoint[4]
                                    - (tempP[0] + tempP[1] + tempP[2] + tempP[3] + point[7]);
                    // 剩余行动次数
                    int times = MAX_ROWCOUNT - this.dgvRear.RowCount;
                    // 14岁普通最有效率的学习平均每次获得点数4260/493 = 8.64;卖萌旅游336/15 = 22.4
                    if (sumPoint > (times * 8.6) && sumPoint < (times * 22))
                    {
                        //普通的工作生活不能满足学满4大属性的时候，需要卖萌旅游
                        moneyCost = true;
                    }
                    else
                    {
                        moneyCost = false;
                    }

                }

                #endregion

                // 有养育金并且行动后名气不到目标属性的情况下，去皇宫生活获得名气
                if (point[4] <= (targetPoint[4] - 26 - equipPoint[4]) && resource[1] > 120)
                {
                    this.cboAction.SelectedIndex = 2;
                    this.cboDetail.SelectedIndex = 2;
                }
                // 疲劳值小于300，行动后叛逆达不到目标属性的情况下，做家务
                else if (resource[0] < MAX_TIRED && point[5] <= (targetPoint[5] - 20 - equipPoint[5]))
                {
                    this.cboAction.SelectedIndex = 0;
                    this.cboDetail.SelectedIndex = 0;

                }

                #region "生活"

                // 疲劳大于等于300，生活减疲劳
                else if (resource[0] >= MAX_TIRED)
                {
                    this.cboAction.SelectedIndex = 2;
                    // 旅游
                    if (currentAge > 13)
                    {
                        // 已达到目标道德属性点，并且普通生活达不到目标属性时需要旅游
                        if (moneyCost && point[6] >= targetPoint[6])
                        {
                            this.cboDetail.SelectedIndex = this.cboDetail.Items.Count - 2;
                        }
                        else
                        {
                            this.cboDetail.SelectedIndex = this.cboDetail.Items.Count - 1;
                        }
                        // 道德获取
                        if (this.chkPay.Enabled && point[6] < targetPoint[6] && resource[1] >= 360)
                        {
                            this.chkPay.Checked = true;
                        }
                        else
                        {
                            this.chkPay.Checked = false;
                        }

                    }
                    else
                    {
                        this.cboDetail.SelectedIndex = this.cboDetail.Items.Count - 1;// 选择最有效率的生活
                    }
                }

                #endregion

                #region "工作"

                // 金钱不足(13岁后是否需要留足养育金获得道德)，工作
                else if ((resource[1] < 120 && currentAge < 13)
                        || (currentAge >= 13
                            && point[6] > targetPoint[6] - 50
                            && resource[1] < 120)
                        || (currentAge >= 13
                            && resource[1] < 360
                            && point[6] <= targetPoint[6] - 50))
                {
                    this.cboAction.SelectedIndex = 0;
                    // 卖萌
                    if (currentAge > 13)
                    {
                        if (moneyCost)
                        {
                            this.cboDetail.SelectedIndex = this.cboDetail.Items.Count - 2;
                        }
                        else
                        {
                            this.cboDetail.SelectedIndex = this.cboDetail.Items.Count - 1;
                        }
                    }
                    else
                    {
                        if (currentAge == 12)
                        {
                            // 优化：黑市小贩工作不如地府引路
                            this.cboDetail.SelectedIndex = this.cboDetail.Items.Count - 2;
                        }
                        else
                        {
                            this.cboDetail.SelectedIndex = this.cboDetail.Items.Count - 1;// 选择最有效率的工作
                        }
                    }
                }

                #endregion

                #region "学习"

                // 学习（耐力）
                else if (resource[1] >= 120 && point[1] <= (targetPoint[1] - 20 - equipPoint[1] - POINT_LEFT))
                {
                    this.cboAction.SelectedIndex = 1;
                    // 低级学习满30次才能进行高级学习
                    if (learnCount[1] < 30)
                    {
                        this.cboDetail.SelectedIndex = 1;
                    }
                    else
                    {
                        this.cboDetail.SelectedIndex = 5;
                    }
                }
                // 学习（内力）
                else if (resource[1] >= 120 && point[3] <= (targetPoint[3] - 20 - equipPoint[3] - POINT_LEFT))
                {
                    this.cboAction.SelectedIndex = 1;
                    // 低级学习满30次才能进行高级学习
                    if (learnCount[3] < 30)
                    {
                        this.cboDetail.SelectedIndex = 3;
                    }
                    else
                    {
                        this.cboDetail.SelectedIndex = 7;
                    }
                }
                // 学习（智力）
                else if (resource[1] >= 120 && point[2] <= (targetPoint[2] - 20 - equipPoint[2] - POINT_LEFT))
                {
                    this.cboAction.SelectedIndex = 1;
                    if (learnCount[2] < 30)
                    {
                        this.cboDetail.SelectedIndex = 2;
                    }
                    else
                    {
                        this.cboDetail.SelectedIndex = 6;
                    }
                }
                // 学习（气质）
                else if (resource[1] >= 120 && point[0] <= (targetPoint[0] - 20 - equipPoint[0] - POINT_LEFT))
                {
                    this.cboAction.SelectedIndex = 1;
                    if (learnCount[0] < 30)
                    {
                        this.cboDetail.SelectedIndex = 0;
                    }
                    else
                    {
                        this.cboDetail.SelectedIndex = 4;
                    }
                }

                #endregion

                // 考虑卖萌旅游后已经达到属性点要求的情况下，补足没达到上限(1200)的属性点
                else if (resource[0] <= MAX_TIRED)
                {
                    #region "补足学习"

                    // 学习（耐力）
                    if (resource[1] >= 120 && point[1] <= (MAX_POINT - 20 - equipPoint[1]))
                    {
                        this.cboAction.SelectedIndex = 1;
                        // 低级学习满30次才能进行高级学习
                        if (learnCount[1] < 30)
                        {
                            this.cboDetail.SelectedIndex = 1;
                        }
                        else
                        {
                            this.cboDetail.SelectedIndex = 5;
                        }
                    }
                    // 学习（内力）
                    else if (resource[1] >= 120 && point[3] <= (MAX_POINT - 20 - equipPoint[3]))
                    {
                        this.cboAction.SelectedIndex = 1;
                        // 低级学习满30次才能进行高级学习
                        if (learnCount[3] < 30)
                        {
                            this.cboDetail.SelectedIndex = 3;
                        }
                        else
                        {
                            this.cboDetail.SelectedIndex = 7;
                        }
                    }
                    // 学习（智力）
                    else if (resource[1] >= 120 && point[2] <= (MAX_POINT - 20 - equipPoint[2]))
                    {
                        this.cboAction.SelectedIndex = 1;
                        if (learnCount[2] < 30)
                        {
                            this.cboDetail.SelectedIndex = 2;
                        }
                        else
                        {
                            this.cboDetail.SelectedIndex = 6;
                        }
                    }
                    // 学习（气质）
                    else if (resource[1] >= 120 && point[0] <= (MAX_POINT - 20 - equipPoint[0]))
                    {
                        this.cboAction.SelectedIndex = 1;
                        if (learnCount[0] < 30)
                        {
                            this.cboDetail.SelectedIndex = 0;
                        }
                        else
                        {
                            this.cboDetail.SelectedIndex = 4;
                        }
                    }
                    // 叛逆
                    else if (point[5] < (MAX_POINT - 20 - equipPoint[5]))
                    {
                        this.cboAction.SelectedIndex = 0;
                        this.cboDetail.SelectedIndex = 0;
                    }
                    // 道德
                    else if (point[6] < (MAX_POINT - 50 - equipPoint[6]))
                    {
                        this.cboAction.SelectedIndex = 0;
                        this.cboDetail.SelectedIndex = this.cboDetail.Items.Count - 1;
                        // 道德获取
                        if (this.chkPay.Enabled && resource[1] >= 360)
                        {
                            this.chkPay.Checked = true;
                        }
                        else
                        {
                            this.chkPay.Checked = false;
                        }
                    }
                    else
                    {
                        error = true;
                    }

                    #endregion
                }
                else
                {
                    error = true;
                }
                // 没有演算错误的情况下，执行下一次行动
                if (!error)
                {
                    this.btnnext_Click(this.btnNext, new EventArgs());
                }

            }
        }

        #endregion

        #endregion

        #endregion

        #region "方法"

        /// <summary>
        /// 得到所在行的年龄
        /// </summary>
        /// <param name="rowIndex">行下标</param>
        /// <returns>年龄</returns>
        private int getAge(int rowIndex)
        {
            int currentAge = 0;
            currentAge = 6 + rowIndex / 3 / 12;
            return currentAge;
        }

        /// <summary>
        /// 具体行动下拉框内容控制
        /// </summary>
        /// <param name="age">年龄</param>
        /// <param name="action">行动方式</param>
        /// <returns>能进行的行动个数</returns>
        private int detailDropControl(int age, string action) 
        {
            // 工作
            if (action == actionGroup[0]) {
                if (age >= 14)
                {
                    return work.Length;
                }
                else if(age >= 13) 
                {
                    return work.Length - 1;
                }
                else if (age >= 12)
                {
                    return work.Length - 3;
                }
                else if (age >= 10)
                {
                    return work.Length - 4;
                }
                else if (age >= 9)
                {
                    return work.Length - 5;
                }
                else if (age >= 8)
                {
                    return work.Length - 6;
                }
                else if (age >= 7)
                {
                    return work.Length - 7;
                }
                else
                {
                    return work.Length - 9;
                }
                
            }
            // 学习的控制，高级学习需要低级学习30次，用户自己控制？
            if (action == actionGroup[1]) {
                // TODO: necessary?
            }
            // 生活
            if (action == actionGroup[2]) {
                if (age >= 14)
                {
                    return live.Length;
                }
                else if (age >= 13)
                {
                    return live.Length - 1;
                }
                else if (age >= 12)
                {
                    return live.Length - 3;
                }
                else if (age >= 10)
                {
                    return live.Length - 4;
                }
                else if (age >= 9)
                {
                    return live.Length - 5;
                }
                else if (age >= 8)
                {
                    return live.Length - 6;
                }
                else if (age >= 7)
                {
                    return live.Length - 7;
                }
                else
                {
                    return live.Length - 8;
                }
            }
            return 0;
        }

        /// <summary>
        /// 行动数据计算
        /// </summary>
        /// <param name="action">行动</param>
        /// <param name="detailAction">具体的行动</param>
        private void valueControl(string action, string detailAction) {
            // 工作
            if(action == actionGroup[0])
            {
                currentResource[0] = currentResource[0] + 18;
                if (detailAction == work[0])
                {
                    currentPoint[0] -= 2;
                    currentResource[1] += + 10;
                    currentPoint[5] += 20;
                }
                else if (detailAction == work[1])
                {
                    currentResource[1] += 30;
                    currentPoint[6] -= 2;
                }
                else if (detailAction == work[2])
                {
                    currentResource[1] += 30;
                }
                else if (detailAction == work[3])
                {
                    currentResource[1] += 50;
                }
                else if (detailAction == work[4])
                {
                    currentResource[1] += 80;
                }
                else if (detailAction == work[5])
                {
                    currentResource[1] += 100;
                }
                else if (detailAction == work[6])
                {
                    currentResource[1] += 120;
                }
                else if (detailAction == work[7])
                {
                    currentResource[1] += 160;
                    currentPoint[1] -= 2;
                    currentPoint[4] -= 2;
                    currentPoint[3] -= 2;
                }
                else if (detailAction == work[8])
                {
                    currentResource[1] += 190;
                    currentPoint[0] -= 1;
                    currentPoint[2] -= 3;
                    currentPoint[6] -= 3;
                }
                else if (detailAction == work[9])
                {
                    currentResource[1] += 900;
                }
                else if (detailAction == work[10])
                {
                    currentResource[1] += 220;
                    currentPoint[0] -= 2;
                    currentPoint[4] -= 2;
                    currentPoint[6] -= 2;
                }
            }
            // 学习
            else if (action == actionGroup[1])
            {
                currentResource[0] += 18;
                currentResource[1] -= 120;
                if (detailAction == learn[0]) 
                {
                    currentPoint[0] += 24;
                }
                else if (detailAction == learn[1])
                {
                    currentPoint[1] += 24;
                }
                else if (detailAction == learn[2])
                {
                    currentPoint[2] += 24;
                }
                else if (detailAction == learn[3])
                {
                    currentPoint[3] += 24;
                }
                else if (detailAction == learn[4])
                {
                    currentPoint[0] += 22;
                    currentPoint[1] += 2;
                    currentPoint[2] += 2;
                }
                else if (detailAction == learn[5])
                {
                    currentPoint[1] += 22;
                    currentPoint[2] += 2;
                    currentPoint[3] += 2;
                }
                else if (detailAction == learn[6])
                {
                    currentPoint[2] += 22;
                    currentPoint[0] += 2;
                    currentPoint[3] += 2;
                }
                else if (detailAction == learn[7])
                {
                    currentPoint[3] += 22;
                    currentPoint[0] += 2;
                    currentPoint[1] += 2;
                }
            }
            // 生活
            else if (action == actionGroup[2]) 
            {
                if (detailAction == live[0])
                {
                    currentResource[0] -= 20;
                }
                else if (detailAction == live[1])
                {
                    currentResource[0] -= 20;
                }
                else if (detailAction == live[2])
                {
                    currentPoint[4] += 26;
                    currentResource[1] -= 120; 
                }
                else if (detailAction == live[3])
                {
                    currentResource[0] -= 24;
                }
                else if (detailAction == live[4])
                {
                    currentResource[0] -= 28;
                }
                else if (detailAction == live[5])
                {
                    currentResource[0] -= 32;
                }
                else if (detailAction == live[6])
                {
                    currentResource[0] -= 36;
                    currentPoint[7] += 1;
                    currentPoint[4] -= 1; 
                }
                else if (detailAction == live[7])
                {
                    currentResource[0] -= 38;
                    currentPoint[7] += 2;
                    currentPoint[2] -= 2;
                }
                else if (detailAction == live[8])
                {
                    currentResource[0] -= 40;
                    currentPoint[7] += 2;
                    currentPoint[6] -= 2;
                }
                else if (detailAction == live[9])
                {
                    currentResource[0] -= 280;
                }
                else if (detailAction == live[10])
                {
                    currentResource[0] -= 40;
                    currentPoint[7] += 2;
                    currentPoint[0] -= 2;
                }
            }

            if (this.chkPay.Checked && this.chkPay.Enabled)
            {
                currentPoint[6] += 50;
                currentResource[1] -= 360;
            }

            for (int i = 0; i < currentPoint.Length; i++) 
            {
                if (currentPoint[i] < 0)
                {
                    currentPoint[i] = 0;
                }
                else if (currentPoint[i] > MAX_POINT)
                {
                    currentPoint[i] = MAX_POINT;
                }
            }
            if (currentResource[0] < 0)
            {
                zerotired = true;
                currentResource[0] = 0;
            }
            else
            {
                zerotired = false;
            }

        }

        /// <summary>
        /// 设置表格数据
        /// </summary>
        /// <param name="rowIndex">行的下标</param>
        private void setCellValue(int rowIndex) 
        {
            for (int i = 1; i <= 7; i++)
            {
                if (currentPoint[i - 1] + equipPoint[i - 1] > MAX_POINT)
                {
                    this.dgvRear.Rows[rowIndex].Cells[i].Value = MAX_POINT;
                }
                else
                {
                    this.dgvRear.Rows[rowIndex].Cells[i].Value = currentPoint[i - 1] + equipPoint[i - 1];
                }
            }
            // 玩性
            this.dgvRear.Rows[rowIndex].Cells[8].Value = currentPoint[currentPoint.Length - 1];
            // 疲劳
            this.dgvRear.Rows[rowIndex].Cells[12].Value = currentResource[0];
            // 养育金
            this.dgvRear.Rows[rowIndex].Cells[13].Value = currentResource[1];
            // 行动
            DataGridViewTextBoxCell cell9 = (DataGridViewTextBoxCell)this.dgvRear.Rows[rowIndex].Cells[9];
            cell9.Value = this.cboAction.Text;
            // 具体行动
            DataGridViewTextBoxCell cell10 = (DataGridViewTextBoxCell)this.dgvRear.Rows[rowIndex].Cells[10];
            cell10.Value = this.cboDetail.Text;
            // 是否给乞丐钱
            DataGridViewTextBoxCell cell11 = (DataGridViewTextBoxCell)this.dgvRear.Rows[rowIndex].Cells[11];

            if (this.chkPay.Checked && this.chkPay.Enabled)
            {
                cell11.Value = "给";
            }
            else
            {
                cell11.Value = "";
            }
        }

        /// <summary>
        /// 初始化
        /// </summary>
        private void initial()
        {
            // 0疲劳警告
            zerotired = false;
            // 行动方式
            this.cboAction.Enabled = true;
            // 具体行动，清空，以防例如6岁可以去赌场生活的事件
            this.cboDetail.Enabled = true;
            this.cboDetail.SelectedItem = null;
            this.cboDetail.Items.Clear();
            // 给乞丐钱选择框不可用
            this.chkPay.Enabled = false;

            int value = 0;
            int month = 0;

            // 获得距离6岁的月份数
            month = parseValue(this.tbxmonth.Text);
            if (month < 0)
            {
                MessageBox.Show("孩子距离6岁的月份数不能为负数！");
                return;
            }
            if (month * 3 > MAX_ROWCOUNT - 1)
            {
                int warnMonth = MAX_ROWCOUNT / 3 - 1;
                MessageBox.Show("只模拟成年前的数据，孩子距离6岁的月份数不能大于" + warnMonth + "。");
                this.tbxmonth.Focus();
                return;
            }
            // 清空表数据
            this.dgvRear.Rows.Clear();
            // 设置表行数
            for (int i = 0; i <= month * 3; i++)
            {
                this.dgvRear.Rows.Add();
            }

            // 初始月份记录；
            startMonth = month;

            // 行动次数统计
            workCount = new int[work.Length];
            learnCount = new int[learn.Length];
            liveCount = new int[live.Length + 1];// +1 是为了给乞丐钱的事件

            // 装备点数
            value = parseValue(this.tbxepqiz.Text);
            equipPoint[0] = value;
            value = parseValue(this.tbxepnal.Text);
            equipPoint[1] = value;
            value = parseValue(this.tbxepzl.Text);
            equipPoint[2] = value;
            value = parseValue(this.tbxepnl.Text);
            equipPoint[3] = value;
            value = parseValue(this.tbxepmq.Text);
            equipPoint[4] = value;
            value = parseValue(this.tbxeppn.Text);
            equipPoint[5] = value;
            value = parseValue(this.tbxepdd.Text);
            equipPoint[6] = value;

            // 初始数据
            value = parseValue(this.tbxqiz.Text);
            point[0] = value;
            value = parseValue(this.tbxnal.Text);
            point[1] = value;
            value = parseValue(this.tbxzl.Text);
            point[2] = value;
            value = parseValue(this.tbxnl.Text);
            point[3] = value;
            value = parseValue(this.tbxmq.Text);
            point[4] = value;
            value = parseValue(this.tbxpn.Text);
            point[5] = value;
            value = parseValue(this.tbxdd.Text);
            point[6] = value;
            value = parseValue(this.tbxwx.Text);
            point[7] = value;

            // 起始点数记录
            for (int i = 0; i < startPoint.Length; i++)
            {
                startPoint[i] = point[i];
            }

            // 初始资源
            value = parseValue(this.tbxtired.Text);
            resource[0] = value;
            value = parseValue(this.tbxmoney.Text);
            resource[1] = value;

            // 初始资源记录
            startResource[0] = resource[0];
            startResource[1] = resource[1];

            // 行数据读取
            dataCopy(true);
            // 设置初始行数据
            setCellValue(month * 3);
        }

        /// <summary>
        /// 行动操作后，设置行数据
        /// </summary>
        private void opeartion()
        {
            // 读取数据
            dataCopy(true);
            // 控制数值
            valueControl(this.cboAction.Text, this.cboDetail.Text);
            // 设置行数据（最后一行）
            setCellValue(this.dgvRear.RowCount - 1);
        }

        /// <summary>
        /// 当前行数据的读取与保存
        /// </summary>
        /// <param name="toCurrent">
        /// <para>ture:读取</para>
        /// <para>false:保存</para>
        /// </param>
        private void dataCopy(bool toCurrent)
        {
            for (int i = 0; i < point.Length; i++)
            {
                if (toCurrent)
                {
                    currentPoint[i] = point[i];
                }
                else
                {
                    point[i] = currentPoint[i];
                }
            }
            for (int i = 0; i < resource.Length; i++)
            {
                if (toCurrent)
                {
                    currentResource[i] = resource[i];
                }
                else
                {
                    resource[i] = currentResource[i];
                }
            }
        }

        /// <summary>
        /// string文本内容转int
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        private int parseValue(string text)
        {
            int parseResult = 0;

            if ("".Equals(text))
            {
                return 0;
            }

            try
            {
                parseResult = int.Parse(text);
            }
            catch (Exception e)
            {
                MessageBox.Show("输入的文本不是数字！\n\n" + e.ToString(), "format error", MessageBoxButtons.OK);
            }

            return parseResult;
        }

        /// <summary>
        /// 行动次数统计（传入行）
        /// </summary>
        /// <param name="rowIndex">行下标</param>
        private void actionCount(int rowIndex)
        {
            DataGridViewTextBoxCell cell10 = (DataGridViewTextBoxCell)this.dgvRear.Rows[rowIndex].Cells[10];
            DataGridViewTextBoxCell cell11 = (DataGridViewTextBoxCell)this.dgvRear.Rows[rowIndex].Cells[11];
            for (int i = 0; i < work.Length; i++)
            {
                if (cell10.Value != null && cell10.Value.ToString() == work[i])
                {
                    workCount[i]++;
                }
            }
            for (int i = 0; i < learn.Length; i++)
            {
                if (cell10.Value != null && cell10.Value.ToString() == learn[i])
                {
                    learnCount[i]++;
                }
            }
            for (int i = 0; i < live.Length; i++)
            {
                if (cell10.Value != null && cell10.Value.ToString() == live[i])
                {
                    liveCount[i]++;
                }
            }
            // 给乞丐事件
            if (cell11.Value != null && !"".Equals(cell11.Value.ToString()))
            {
                liveCount[liveCount.Length - 1]++;
            }
        }

        /// <summary>
        /// 行动次数统计显示在表格中
        /// </summary>
        private void displayCount()
        {
            StringBuilder strValue = new StringBuilder();
            foreach (string item in work)
            {
                strValue.Append(item);
                strValue.Append("\t");
            }
            strValue.Remove(strValue.Length - 1, 1);//移除掉最后一个,字符
            strValue.Append("\r\n");
            foreach (int item in workCount)
            {
                strValue.Append(item);
                strValue.Append("\t");
            }
            strValue.Remove(strValue.Length - 1, 1);//移除掉最后一个,字符
            strValue.Append("\r\n");
            foreach (string item in learn)
            {
                strValue.Append(item);
                strValue.Append("\t");
            }
            strValue.Remove(strValue.Length - 1, 1);//移除掉最后一个,字符
            strValue.Append("\r\n");
            foreach (int item in learnCount)
            {
                strValue.Append(item);
                strValue.Append("\t");
            }
            strValue.Remove(strValue.Length - 1, 1);//移除掉最后一个,字符
            strValue.Append("\r\n");
            foreach (string item in live)
            {
                strValue.Append(item);
                strValue.Append("\t");
            }
            strValue.Append("给乞丐钱");
            strValue.Append("\r\n");
            foreach (int item in liveCount)
            {
                strValue.Append(item);
                strValue.Append("\t");
            }
            strValue.Remove(strValue.Length - 1, 1);//移除掉最后一个,字符
            this.tbxActionCount.Text = strValue.ToString();
        }

        /// <summary>
        /// 职业评价计算
        /// </summary>
        /// <param name="calPoint">7大属性点</param>
        /// <param name="wxPoint">玩性</param>
        /// <returns>职业评价</returns>
        private double calc(int[] calPoint, int wxPoint)
        {
            double evaluate = 0;
            if (calPoint.Length != 7)
            {
                return 0;
            }
            Array.Sort(calPoint);
            evaluate = 600 * Math.Pow(calPoint[6], 0.2)
                        + 180 * Math.Pow(calPoint[5], 0.2)
                        + 150 * Math.Pow(calPoint[4], 0.2)
                        + 120 * Math.Pow(calPoint[3], 0.2)
                        + 90 * Math.Pow(calPoint[2], 0.2)
                        + 60 * Math.Pow(calPoint[1], 0.2)
                        + 30 * Math.Pow(calPoint[0], 0.2);
            return evaluate - wxPoint * 20;
        }

        #endregion

    }
}
