using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace RearInventor
{
    class FileHelper
    {
        /// <summary>
        /// 属性点说明文本
        /// </summary>
        private string[] pointComment = { "气质", "耐力", "智力", "内力", "名气", "叛逆", "道德", "玩性" };

        /// <summary>
        /// 资源说明文本
        /// </summary>
        private string[] resourceComment = { "疲劳", "养育金" };

        /// <summary>
        /// 分隔符
        /// </summary>
        public const string separative = "\t";

        /// <summary>
        /// 文本最大行数
        /// </summary>
        private const int MAX_LINE = 442;

        /// <summary>
        /// 文本最小行数
        /// </summary>
        private const int MIN_LINE = 10;

        /// <summary>
        /// 导出方法
        /// </summary>
        /// <param name="dt">表格数据</param>
        /// <param name="strFileName">文件路径</param>
        /// <param name="startMonth">起始月份</param>
        /// <param name="startPoint">初始点数</param>
        /// <param name="startResource">初始资源</param>
        /// <param name="equipPoint">装备点数</param>
        /// <returns>true:导出成功; false:失败</returns>
        public bool ExportToFile(DataGridView dt, string strPath, int startMonth, int[] startPoint, int[] startResource, int[] equipPoint)
        {
            if(File.Exists(strPath))
            {
                File.Delete(strPath);
            }
            StringBuilder strStartValue = new StringBuilder();
            StringBuilder strColu = new StringBuilder();
            StringBuilder strValue = new StringBuilder();
            int i = 0;
            try
            {
                StreamWriter sw = new StreamWriter(new FileStream(strPath, FileMode.CreateNew), Encoding.GetEncoding("UTF-8"));

                sw.WriteLine("起始月份");
                sw.WriteLine(startMonth);

                // 起始属性文本
                for (i = 0; i < pointComment.Length; i++)
                {
                    strStartValue.Append(pointComment[i]);
                    strStartValue.Append(separative);
                }
                strStartValue.Remove(strStartValue.Length - 1, 1);//移除掉最后一个,字符
                sw.WriteLine(strStartValue);//字段名写入文件
                // 清空
                strStartValue.Remove(0, strStartValue.Length);
                // 起始属性值
                for (i = 0; i < startPoint.Length; i++)
                {
                    strStartValue.Append(startPoint[i]);
                    strStartValue.Append(separative);
                }
                strStartValue.Remove(strStartValue.Length - 1, 1);//移除掉最后一个,字符
                sw.WriteLine(strStartValue);
                // 清空
                strStartValue.Remove(0, strStartValue.Length);

                // 起始资源文本
                for (i = 0; i < resourceComment.Length; i++)
                {
                    strStartValue.Append(resourceComment[i]);
                    strStartValue.Append(separative);
                }
                strStartValue.Remove(strStartValue.Length - 1, 1);//移除掉最后一个,字符
                sw.WriteLine(strStartValue);//字段名写入文件
                // 清空
                strStartValue.Remove(0, strStartValue.Length);
                // 起始资源值
                for (i = 0; i < startResource.Length; i++)
                {
                    strStartValue.Append(startResource[i]);
                    strStartValue.Append(separative);
                }
                strStartValue.Remove(strStartValue.Length - 1, 1);//移除掉最后一个,字符
                sw.WriteLine(strStartValue);
                // 清空
                strStartValue.Remove(0, strStartValue.Length);

                // 装备点数文本，没有“玩性”
                for (i = 0; i < pointComment.Length - 1; i++)
                {
                    strStartValue.Append(pointComment[i]);
                    strStartValue.Append(separative);
                }
                strStartValue.Remove(strStartValue.Length - 1, 1);//移除掉最后一个,字符
                sw.WriteLine(strStartValue);//字段名写入文件
                // 清空
                strStartValue.Remove(0, strStartValue.Length);
                // 装备属性值
                for (i = 0; i < equipPoint.Length; i++)
                {
                    strStartValue.Append(equipPoint[i]);
                    strStartValue.Append(separative);
                }
                strStartValue.Remove(strStartValue.Length - 1, 1);//移除掉最后一个,字符
                sw.WriteLine(strStartValue);

                // 表格头部
                for (i = 0; i <= dt.Columns.Count - 1; i++)
                {
                    strColu.Append(dt.Columns[i].Name);
                    strColu.Append(separative);//CSV的分隔符
                }
                strColu.Remove(strColu.Length - 1, 1);//移除掉最后一个,字符
                sw.WriteLine(strColu);//字段名写入文件
                foreach (DataGridViewRow dr in dt.Rows)
                {
                    strValue.Remove(0, strValue.Length);//清空字符串
                    for (i = 0; i <= dt.Columns.Count - 1; i++)
                    {
                        if (dr.Cells[i].Value != null)
                        {
                            strValue.Append(dr.Cells[i].Value.ToString());
                        }
                        strValue.Append(separative);
                    }
                    strValue.Remove(strValue.Length - 1, 1);//移除掉最后一个,字符
                    sw.WriteLine(strValue);
                }
                sw.Close();
            }
            catch (Exception ex)
            {
                return false;
                throw ex;
            }
            return true;
        }

        /// <summary>
        /// 读取文件
        /// </summary>
        /// <param name="strPath">文件路径</param>
        /// <returns>文件内容ArrayList</returns>
        public ArrayList ImportFile(string strPath)
        {
            if (!File.Exists(strPath))
            {
                 //TODO:文件不存在
                return null;
            }
            StreamReader objReader = new StreamReader(strPath);
            string sLine = "";
            ArrayList LineList = new ArrayList();
            while (sLine != null)
            {
                sLine = objReader.ReadLine();
                if (sLine != null && !"".Equals(sLine))
                {
                    LineList.Add(sLine);
                }
            }
            objReader.Close();
            if (LineList.Count > MAX_LINE)
            {
                return null;
            }
            else if (LineList.Count < MIN_LINE)
            {
                return null;
            }
            return LineList;
        }
        
        
    }
}
