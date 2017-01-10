#pragma once

#include <windows.h>

namespace mfc{
    class screen
    {
    public:
        screen(){}
        ~screen(){}
        static HBITMAP CopyScreenToBitmap(LPRECT lpRect) //lpRect ����ѡ������  
        {
            HDC hScrDC, hMemDC;
            // ��Ļ���ڴ��豸������  
            HBITMAP hBitmap, hOldBitmap;
            // λͼ���  
            int       nX, nY, nX2, nY2;
            // ѡ����������  
            int       nWidth, nHeight;
            // λͼ���Ⱥ͸߶�  
            int       xScrn, yScrn;
            // ��Ļ�ֱ���  
            // ȷ��ѡ������Ϊ�վ���  
            if (IsRectEmpty(lpRect))
                return NULL;
            //Ϊ��Ļ�����豸������  
            hScrDC = CreateDCA("DISPLAY", NULL, NULL, NULL);
            //Ϊ��Ļ�豸�������������ݵ��ڴ��豸������  
            hMemDC = CreateCompatibleDC(hScrDC);
            // ���ѡ����������  
            nX = lpRect->left;
            nY = lpRect->top;
            nX2 = lpRect->right;
            nY2 = lpRect->bottom;
            // �����Ļ�ֱ���  
            xScrn = GetDeviceCaps(hScrDC, HORZRES);
            yScrn = GetDeviceCaps(hScrDC, VERTRES);
            //ȷ��ѡ�������ǿɼ���  
            if (nX < 0)
                nX = 0;
            if (nY < 0)
                nY = 0;
            if (nX2 > xScrn)
                nX2 = xScrn;
            if (nY2 > yScrn)
                nY2 = yScrn;
            nWidth = nX2 - nX;
            nHeight = nY2 - nY;
            // ����һ������Ļ�豸���������ݵ�λͼ  
            hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
            // ����λͼѡ���ڴ��豸��������  
            hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
            // ����Ļ�豸�������������ڴ��豸��������  
            BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);
            //�õ���Ļλͼ�ľ��  
            hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
            //���   
            DeleteDC(hScrDC);
            DeleteDC(hMemDC);
            // ����λͼ���  
            return hBitmap;
        }
        static HBITMAP CopyExeScreenToBitmap(CWnd* wnd, LPRECT lpRect) //lpRect ����ѡ������  
        {
            HDC hScrDC, hMemDC;
            // ��Ļ���ڴ��豸������  
            HBITMAP hBitmap, hOldBitmap;
            // λͼ���  
            int       nX, nY, nX2, nY2;
            // ѡ����������  
            int       nWidth, nHeight;
            // λͼ���Ⱥ͸߶�  
            int       xScrn, yScrn;
            // ��Ļ�ֱ���  
            // ȷ��ѡ������Ϊ�վ���  
            if (IsRectEmpty(lpRect))
                return NULL;
            //Ϊ��Ļ�����豸������  
            hScrDC = wnd->GetDC()->m_hDC;
//             hScrDC = CreateDCA("DISPLAY", NULL, NULL, NULL);
            //Ϊ��Ļ�豸�������������ݵ��ڴ��豸������  
            hMemDC = CreateCompatibleDC(hScrDC);
            // ���ѡ����������  
            nX = lpRect->left;
            nY = lpRect->top;
            nX2 = lpRect->right;
            nY2 = lpRect->bottom;
            // �����Ļ�ֱ���  
            xScrn = GetDeviceCaps(hScrDC, HORZRES);
            yScrn = GetDeviceCaps(hScrDC, VERTRES);
            //ȷ��ѡ�������ǿɼ���  
            if (nX < 0)
                nX = 0;
            if (nY < 0)
                nY = 0;
            if (nX2 > xScrn)
                nX2 = xScrn;
            if (nY2 > yScrn)
                nY2 = yScrn;
            nWidth = nX2 - nX;
            nHeight = nY2 - nY;
            nWidth = xScrn;
            nHeight = yScrn;
            // ����һ������Ļ�豸���������ݵ�λͼ  
            hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
            // ����λͼѡ���ڴ��豸��������  
            hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
            // ����Ļ�豸�������������ڴ��豸��������  
            BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, 0, 0, SRCCOPY);
            //�õ���Ļλͼ�ľ��  
            hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
            //���   
            DeleteDC(hScrDC);
            DeleteDC(hMemDC);
            // ����λͼ���  
            return hBitmap;
        }

        static BOOL SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName) //hBitmap Ϊ�ղŵ���Ļλͼ���  
        {      //lpFileName Ϊλͼ�ļ���  
            HDC     hDC;
            //�豸������  
            int     iBits;
            //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���  
            WORD    wBitCount;
            //λͼ��ÿ��������ռ�ֽ���  
            //�����ɫ���С�� λͼ�������ֽڴ�С ��  λͼ�ļ���С �� д���ļ��ֽ���  
            DWORD           dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;
            BITMAP          Bitmap;
            //λͼ���Խṹ  
            BITMAPFILEHEADER   bmfHdr;
            //λͼ�ļ�ͷ�ṹ  
            BITMAPINFOHEADER   bi;
            //λͼ��Ϣͷ�ṹ   
            LPBITMAPINFOHEADER lpbi;
            //ָ��λͼ��Ϣͷ�ṹ  
            HANDLE          fh, hDib, hPal;
            HPALETTE     hOldPal = NULL;
            //�����ļ��������ڴ�������ɫ����  

            //����λͼ�ļ�ÿ��������ռ�ֽ���  
            hDC = CreateDCA("DISPLAY", NULL, NULL, NULL);
            iBits = GetDeviceCaps(hDC, BITSPIXEL) *
                GetDeviceCaps(hDC, PLANES);
            DeleteDC(hDC);
            if (iBits <= 1)
                wBitCount = 1;
            else if (iBits <= 4)
                wBitCount = 4;
            else if (iBits <= 8)
                wBitCount = 8;
            else if (iBits <= 24)
                wBitCount = 24;
            else
                wBitCount = 32;
            //�����ɫ���С  
            if (wBitCount <= 8)
                dwPaletteSize = (1 << wBitCount)*sizeof(RGBQUAD);

            //����λͼ��Ϣͷ�ṹ  
            GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
            bi.biSize = sizeof(BITMAPINFOHEADER);
            bi.biWidth = Bitmap.bmWidth;
            bi.biHeight = Bitmap.bmHeight;
            bi.biPlanes = 1;
            bi.biBitCount = wBitCount;
            bi.biCompression = BI_RGB;
            bi.biSizeImage = 0;
            bi.biXPelsPerMeter = 0;
            bi.biYPelsPerMeter = 0;
            bi.biClrUsed = 0;
            bi.biClrImportant = 0;

            dwBmBitsSize = ((Bitmap.bmWidth*wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
            //Ϊλͼ���ݷ����ڴ�  

            /*xxxxxxxx����λͼ��С�ֽ�һ��(����һ����������)xxxxxxxxxxxxxxxxxxxx
            //ÿ��ɨ������ռ���ֽ���Ӧ��Ϊ4���������������㷨Ϊ:
            int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
            if((Bitmap.bmWidth*wBitCount) % 32)
            biWidth++; //�����������ļ�1
            biWidth *= 4;//���������õ���Ϊÿ��ɨ���е��ֽ�����
            dwBmBitsSize = biWidth * Bitmap.bmHeight;//�õ���С
            xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
            hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
            lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
            *lpbi = bi;
            // ������ɫ��     
            hPal = GetStockObject(DEFAULT_PALETTE);
            if (hPal)
            {
                hDC = ::GetDC(NULL);
                hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
                RealizePalette(hDC);
            }
            // ��ȡ�õ�ɫ�����µ�����ֵ  
            GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
            //�ָ���ɫ��     
            if (hOldPal)
            {
                SelectPalette(hDC, hOldPal, TRUE);
                RealizePalette(hDC);
                ::ReleaseDC(NULL, hDC);
            }
            //����λͼ�ļ�      
            fh = CreateFileA(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
            if (fh == INVALID_HANDLE_VALUE)
                return FALSE;
            // ����λͼ�ļ�ͷ  
            bmfHdr.bfType = 0x4D42;  // "BM"  
            dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
            bmfHdr.bfSize = dwDIBSize;
            bmfHdr.bfReserved1 = 0;
            bmfHdr.bfReserved2 = 0;
            bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
            // д��λͼ�ļ�ͷ  
            WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
            // д��λͼ�ļ���������  
            WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize, &dwWritten, NULL);
            //���     
            GlobalUnlock(hDib);
            GlobalFree(hDib);
            CloseHandle(fh);
            return TRUE;
        }
    private:


    };



}