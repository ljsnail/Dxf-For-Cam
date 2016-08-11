#pragma once
#include"GeomClose.h"
//这个类主要是实现把之前的封闭环之间串起来切割使用。
//实现，将封闭环之间排序，对圆这种特殊图形要另外处理，对于圆弧要转换成原来的数据形式
//包含封闭图元之间的过渡直线
//弄个单向链表来存储着一张排样dxf的数据
typedef struct//封闭图元之间的过渡线
{
	double pror_x, poro_y, next_x, next_y;//上一封闭图元的xy和下一封闭图元的起点xy
}TranLine;
typedef struct NestResultData//存储所有的图元的单向链表。
{
	TranLine tranline;
	struct NestResultData*nextNode;//指向下一封闭环的地址
	bool flag;//留着以后作为全局规划时候使用。
};
class GeomForCut
{
public:
	GeomForCut();
	~GeomForCut();
public:
	GeomClose geomclose;
public:
	NestResultData*SetCutOrder;

};

