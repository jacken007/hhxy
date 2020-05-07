#include "DebugMethord.h"
CDebugMethord* CDebugMethord::pInstance = nullptr;
CDebugMethord::CDebugMethord() {
}

void CDebugMethord::DB_DrawRect(Rect rect) {
    DrawNode* drawNode = DrawNode::create();
    drawNode->drawRect(rect.origin, rect.size, Color4F::BLUE);
}