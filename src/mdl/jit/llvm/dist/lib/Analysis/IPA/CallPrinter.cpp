//===- CallPrinter.cpp - DOT printer for call graph -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines '-dot-callgraph', which emit a callgraph.<fnname>.dot
// containing the call graph of a module.
//
// There is also a pass available to directly call dotty ('-view-callgraph').
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/CallPrinter.h"
#include "llvm/Analysis/DOTGraphTraitsPass.h"

using namespace llvm;

namespace llvm {

template<>
struct DOTGraphTraits<CallGraph*> : public DefaultDOTGraphTraits {
  DOTGraphTraits (bool isSimple=false) : DefaultDOTGraphTraits(isSimple) {}

  static MISTD::string getGraphName(CallGraph *Graph) {
    return "Call graph";
  }

  MISTD::string getNodeLabel(CallGraphNode *Node, CallGraph *Graph) {
    if (Function *Func = Node->getFunction())
      return Func->getName();

    return "external node";
  }
};

} // end llvm namespace

namespace {

struct CallGraphViewer
  : public DOTGraphTraitsModuleViewer<CallGraph, true> {
  static char ID;

  CallGraphViewer()
    : DOTGraphTraitsModuleViewer<CallGraph, true>("callgraph", ID) {
    initializeCallGraphViewerPass(*PassRegistry::getPassRegistry());
  }
};

struct CallGraphPrinter
  : public DOTGraphTraitsModulePrinter<CallGraph, true> {
  static char ID;

  CallGraphPrinter()
    : DOTGraphTraitsModulePrinter<CallGraph, true>("callgraph", ID) {
      initializeCallGraphPrinterPass(*PassRegistry::getPassRegistry());
  }
};

} // end anonymous namespace

char CallGraphViewer::ID = 0;
INITIALIZE_PASS(CallGraphViewer, "view-callgraph",
                "View call graph",
                false, false)

char CallGraphPrinter::ID = 0;
INITIALIZE_PASS(CallGraphPrinter, "dot-callgraph",
                "Print call graph to 'dot' file",
                false, false)

// Create methods available outside of this file, to use them
// "include/llvm/LinkAllPasses.h". Otherwise the pass would be deleted by
// the link time optimization.

ModulePass *llvm::createCallGraphViewerPass() {
  return new CallGraphViewer();
}

ModulePass *llvm::createCallGraphPrinterPass() {
  return new CallGraphPrinter();
}
