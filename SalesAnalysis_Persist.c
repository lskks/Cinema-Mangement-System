

#include "SalesAnalysis_Persist.h"

// 统计票房模块本身不需要直接操作文件
// 数据统计依赖于其他模块的持久化层函数
// 此文件保留为空

// 实际的数据读取由以下模块的持久化层完成：
// 1. Play_Perst_SelectAll - 读取剧目信息
// 2. Schedule_Perst_SelectByPlay - 读取演出计划信息
// 3. Ticket_Perst_SelectBySchID - 读取票信息
// 4. Sale_Perst_SelectByTicketID - 读取销售记录