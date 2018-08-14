#include "enfield/Transform/LayerBasedOrderingWrapperPass.h"
#include "enfield/Transform/PassCache.h"
#include "enfield/Support/Defs.h"

uint32_t efd::LayerBasedOrderingWrapperPass::getNodeId(Node::Ref ref) {
    if (mStmtId.find(ref) == mStmtId.end()) {
        std::string refStr = (ref == nullptr) ? "nullptr" : ref->toString(false);
        efd::ERR << "Unknown node: `" << refStr << "`." << std::endl;
        efd::EFD_ABORT();
    }

    return mStmtId[ref];
}

bool efd::LayerBasedOrderingWrapperPass::run(QModule* qmod) {
    mStmtId.clear();

    for (auto it = qmod->stmt_begin(), end = qmod->stmt_end(); it != end; ++it) {
        mStmtId.insert(std::make_pair(it->get(), mStmtId.size()));
    }

    auto cgbpass = PassCache::Get<CircuitGraphBuilderPass>(qmod);
    mData.ordering = generate(cgbpass->getData());
    qmod->orderby(mData.ordering);

    return true;
}
