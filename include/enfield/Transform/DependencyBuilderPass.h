#ifndef __EFD_DEPENDENCY_BUILDER_PASS_H__
#define __EFD_DEPENDENCY_BUILDER_PASS_H__

#include "enfield/Pass.h"
#include "enfield/Analysis/Nodes.h"
#include "enfield/Transform/QModule.h"

#include <unordered_map>
#include <vector>
#include <set>

namespace efd {

    /// \brief Maps every quantum bit (not register) to a number inside a vector.
    ///
    /// It also keeps track of the qbits inside every gate declaration. Inside every
    /// scope, it maps the qbits existing inside them to an unsigned number.
    /// 
    /// Note that if "qreg r[10];" declaration exists, then "r" is not a qbit, but
    /// "r[n]" is (where "n" is in "{0 .. 9}").
    class QbitToNumberPass : public Pass {
        private:
            struct QbitInfo {
                std::string mKey;
                NodeRef mRef;
            };

        public:
            typedef std::vector<QbitInfo> QbitMap;

        private:
            std::unordered_map<NDGateDecl*, QbitMap> mLIdMap;
            QbitMap mGIdMap;

            QbitToNumberPass();

            const QbitMap* getMap(NDGateDecl* gate) const;
            void initImpl(bool force) override;

        public:
            void visit(NDDecl* ref) override;
            void visit(NDGateDecl* ref) override;

            /// \brief Returns an unsigned number representing the id
            /// in this specific gate (if any).
            unsigned getUId(std::string id, NDGateDecl* gate = nullptr) const;

            /// \brief Returns the number of qbits in a given gate (if any).
            unsigned getSize(NDGateDecl* gate = nullptr) const;

            /// \brief Returns the std::string id representation of the
            /// corresponding unsigned id in the specific gate (if any).
            std::string getStrId(unsigned id, NDGateDecl* gate = nullptr) const;

            /// \brief Get a NodeRef, representing that qbit.
            NodeRef getNode(unsigned id, NDGateDecl* gate = nullptr) const;

            /// \brief Returns a new instance of this class.
            static QbitToNumberPass* Create();
    };

    /// \brief Structure for abstracting dependencies.
    struct Dep {
        unsigned mFrom;
        unsigned mTo;
    };

    /// \brief Represents a sequence of dependencies (should be treated as
    /// parallel dependencies) for each node.
    struct Dependencies {
        typedef std::vector<Dep>::iterator Iterator;
        typedef std::vector<Dep>::const_iterator ConstIterator;

        std::vector<Dep> mDeps;
        NodeRef mCallPoint;

        /// \brief Forwards to the \em mDeps attribute.
        const Dep& operator[](unsigned i) const;
        /// \brief Forwards to the \em mDeps attribute.
        Dep& operator[](unsigned i);

        /// \brief Forwards to the \em mDeps attribute.
        bool isEmpty() const;

        /// \brief Forwards to the \em mDeps attribute.
        unsigned getSize() const;

        /// \brief Forwards to the \em mDeps attribute.
        Iterator begin();
        /// \brief Forwards to the \em mDeps attribute.
        ConstIterator begin() const;
        /// \brief Forwards to the \em mDeps attribute.
        Iterator end();
        /// \brief Forwards to the \em mDeps attribute.
        ConstIterator end() const;
    };

    /// \brief Keep track of the dependencies of each qbit for the whole program,
    /// as well as the dependencies for every gate.
    ///
    /// Each gate, as well as the whole program have one 'DepsSet' variable. The idea is
    /// to store a sequence of parallel dependencies. Here, parallel dependency is a
    /// dependency that can't be broken down (unless the gate is inlined).
    class DependencyBuilderPass : public Pass {
        public:
            typedef std::vector<Dependencies> DepsSet;

        private:
            QModule* mMod;
            NDGateDecl* mCurrentGate;
            QbitToNumberPass* mQbitMap;

            std::unordered_map<NDGateDecl*, DepsSet> mLDeps;
            DepsSet mGDeps;

            DependencyBuilderPass(QModule* mod, QbitToNumberPass* pass = nullptr);

            /// \brief Gets an unsingned id for \p ref.
            unsigned getUId(NodeRef ref);
            /// \brief Gets the DepsSet corresponding to the current quantum gate,
            /// or the global (if current gate is null).
            const DepsSet* getDepsSet(NDGateDecl* gate = nullptr) const;
            DepsSet* getDepsSet(NDGateDecl* gate = nullptr);

        protected:
            void initImpl(bool force) override;

        public:
            void visit(NDGateDecl* ref) override;
            void visit(NDGOpList* ref) override;
            void visit(NDQOpCX* ref) override;
            void visit(NDQOpGeneric* ref) override;
            void visit(NDIfStmt* ref) override;

            /// \brief Returns the pass that mapped the qbits.
            QbitToNumberPass* getUIdPass();

            /// \brief Gets the dependencies for some gate declaration. If it is a
            /// nullptr, then it is returned the dependencies for the whole program.
            const DepsSet& getDependencies(NDGateDecl* ref = nullptr) const;
            DepsSet getDependencies(NDGateDecl* ref = nullptr);

            /// \brief Returns a new instance of this class.
            static DependencyBuilderPass* Create(QModule* mod, QbitToNumberPass* pass = nullptr);
    };
};

#endif