/*
    �ļ�����    Utils.h
    �衡����    ��Ϸ�ж����һЩ��������
    �����ˣ�    ������ (���ͣ�http://blog.csdn.net/haomengzhu)

    �������ڣ�   2013.12.09
*/
#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"

//ȫ�ֱ���
#define  g_groundHight  50 
#define g_runnerStartX  100

// collision type for chipmunk
enum SpriteTag{
	SpriteTagRunner,
	SpriteTagCoin,
	SpriteTagRock,
};

#endif // __UTILS_H__
