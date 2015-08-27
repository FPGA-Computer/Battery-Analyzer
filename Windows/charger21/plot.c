void drawGraph (HWND hwnd, HDC hdc)
{
	//255-150*(g_dataArrayA[i][j]+1)
	int xAxis[9] = {-180,-150,-120,-90,0,90,120,150,180};
	//float g_yAxis[8] = {0,0.5,1.0,1.5,2.0,2.5,3.0,3.5};
	int i=0;
	HPEN	hPenOld, hPenA, hPenB, hPenC, hPenD;

	MoveToEx(hdc, graphOffset, 0, NULL);
	LineTo(hdc, graphOffset+((360/amount)*(int)floor((16200*amount)/360)), 0);
	

	MoveToEx(hdc, graphOffset+((180/amount)*(int)floor((16200*amount)/360)), 0, NULL);
	LineTo(hdc, graphOffset+((180/amount)*(int)floor((16200*amount)/360)), (int)(g_yAxis[7]*scaleData));

	MoveToEx(hdc, graphOffset, (int)(g_yAxis[1]*scaleData), NULL);
	LineTo(hdc, graphOffset+((360/amount)*(int)floor((16200*amount)/360)), (int)(g_yAxis[1]*scaleData));
	MoveToEx(hdc, graphOffset, (int)(g_yAxis[2]*scaleData), NULL);
	LineTo(hdc, graphOffset+((360/amount)*(int)floor((16200*amount)/360)), (int)(g_yAxis[2]*scaleData));
	MoveToEx(hdc, graphOffset, (int)(g_yAxis[3]*scaleData), NULL);
	LineTo(hdc, graphOffset+((360/amount)*(int)floor((16200*amount)/360)), (int)(g_yAxis[3]*scaleData));
	MoveToEx(hdc, graphOffset, (int)(g_yAxis[4]*scaleData), NULL);
	LineTo(hdc, graphOffset+((360/amount)*(int)floor((16200*amount)/360)), (int)(g_yAxis[4]*scaleData));
	MoveToEx(hdc, graphOffset, (int)(g_yAxis[5]*scaleData), NULL);
	LineTo(hdc, graphOffset+((360/amount)*(int)floor((16200*amount)/360)), (int)(g_yAxis[5]*scaleData));
	MoveToEx(hdc, graphOffset, (int)(g_yAxis[6]*scaleData), NULL);
	LineTo(hdc, graphOffset+((360/amount)*(int)floor((16200*amount)/360)), (int)(g_yAxis[6]*scaleData));
	hPenA=CreatePen(PS_SOLID,30,RGB(0,255,0));
	hPenB=CreatePen(PS_SOLID,30,RGB(255,0,00));
	hPenC=CreatePen(PS_SOLID,30,RGB(0,0,255));
	hPenD=CreatePen(PS_SOLID,30,RGB(255,255,0));
	hPenOld = SelectObject(hdc, hPenA);



	if (g_i>1 && g_i<=(int)(360/amount))//paint the individual points
	{
		
		MoveToEx(hdc, graphOffset+((g_i-1)*(int)floor((16200*amount)/360)), ((g_aAverage[g_i-1])), NULL);
		SelectObject(hdc, hPenA);
		LineTo(hdc, graphOffset+((g_i)*(int)floor((16200*amount)/360)), (g_aAverage[g_i]));
		MoveToEx(hdc, graphOffset+((g_i-1)*(int)floor((16200*amount)/360)), ((g_bAverage[g_i-1])), NULL);
		SelectObject(hdc, hPenB);
		LineTo(hdc, graphOffset+((g_i)*(int)floor((16200*amount)/360)), ((g_bAverage[g_i])));
		MoveToEx(hdc, graphOffset+((g_i-1)*(int)floor((16200*amount)/360)), ((g_cAverage[g_i-1])), NULL);
		SelectObject(hdc, hPenC);
		LineTo(hdc, graphOffset+((g_i)*(int)floor((16200*amount)/360)), ((g_cAverage[g_i])));
		MoveToEx(hdc, graphOffset+((g_i-1)*(int)floor((16200*amount)/360)), ((g_dAverage[g_i-1])), NULL);
		SelectObject(hdc, hPenD);
		LineTo(hdc, graphOffset+((g_i)*(int)floor((16200*amount)/360)), ((g_dAverage[g_i])));
	}
	else if (g_i>=(int)(360/amount)) //repaints the entire graph so as it is not lost to future repaints
	{
		for (i=1;i<=(int)(360/amount);i++)
		{	
			if (i>1)
			{
				MoveToEx(hdc, graphOffset+((i-1)*(int)floor((16200*amount)/360)), ((g_aAverage[i-1])), NULL);
				SelectObject(hdc, hPenA);
				LineTo(hdc, graphOffset+((i)*(int)floor((16200*amount)/360)), ((g_aAverage[i])));
				MoveToEx(hdc, graphOffset+((i-1)*(int)floor((16200*amount)/360)), ((g_bAverage[i-1])), NULL);
				SelectObject(hdc, hPenB);
				LineTo(hdc, graphOffset+((i)*(int)floor((16200*amount)/360)), ((g_bAverage[i])));
				MoveToEx(hdc, graphOffset+((i-1)*(int)floor((16200*amount)/360)), ((g_cAverage[i-1])), NULL);
				SelectObject(hdc, hPenC);
				LineTo(hdc, graphOffset+((i)*(int)floor((16200*amount)/360)), ((g_cAverage[i])));
				MoveToEx(hdc, graphOffset+((i-1)*(int)floor((16200*amount)/360)), ((g_dAverage[i-1])), NULL);
				SelectObject(hdc, hPenD);
				LineTo(hdc, graphOffset+((i)*(int)floor((16200*amount)/360)), ((g_dAverage[i])));
			}
		}
	}
	SelectObject(hdc, hPenOld);
	DeleteObject(hPenA);
	DeleteObject(hPenB);
	DeleteObject(hPenC);
	DeleteObject(hPenD);
}
