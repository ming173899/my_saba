//
//Copyright (C) 2016 Google, Inc.
//Copyright (C) 2016 LunarG, Inc.
//
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//    Neither the name of 3Dlabs Inc. Ltd. nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//POSSIBILITY OF SUCH DAMAGE.
//
#ifndef HLSL_PARSE_INCLUDED_
#define HLSL_PARSE_INCLUDED_

#include "../glslang/MachineIndependent/parseVersions.h"
#include "../glslang/MachineIndependent/ParseHelper.h"

namespace glslang {

class TAttributeMap; // forward declare

class HlslParseContext : public TParseContextBase {
public:
    HlslParseContext(TSymbolTable&, TIntermediate&, bool parsingBuiltins,
                     int version, EProfile, const SpvVersion& spvVersion, EShLanguage, TInfoSink&,
                     bool forwardCompatible = false, EShMessages messages = EShMsgDefault);
    virtual ~HlslParseContext();
    void initializeExtensionBehavior();

    void setLimits(const TBuiltInResource&);
    bool parseShaderStrings(TPpContext&, TInputScanner& input, bool versionWillBeError = false);
    virtual const char* getGlobalUniformBlockName() { return "$Global"; }

    void reservedPpErrorCheck(const TSourceLoc&, const char* /*name*/, const char* /*op*/) { }
    bool lineContinuationCheck(const TSourceLoc&, bool /*endOfComment*/) { return true; }
    bool lineDirectiveShouldSetNextLine() const { return true; }
    bool builtInName(const TString&);

    void handlePragma(const TSourceLoc&, const TVector<TString>&);
    TIntermTyped* handleVariable(const TSourceLoc&, TSymbol* symbol,  const TString* string);
    TIntermTyped* handleBracketDereference(const TSourceLoc&, TIntermTyped* base, TIntermTyped* index);
    TIntermTyped* handleBracketOperator(const TSourceLoc&, TIntermTyped* base, TIntermTyped* index);
    void checkIndex(const TSourceLoc&, const TType&, int& index);

    TIntermTyped* handleBinaryMath(const TSourceLoc&, const char* str, TOperator op, TIntermTyped* left, TIntermTyped* right);
    TIntermTyped* handleUnaryMath(const TSourceLoc&, const char* str, TOperator op, TIntermTyped* childNode);
    TIntermTyped* handleDotDereference(const TSourceLoc&, TIntermTyped* base, const TString& field);
    void assignLocations(TVariable& variable);
    TFunction& handleFunctionDeclarator(const TSourceLoc&, TFunction& function, bool prototype);
    TIntermAggregate* handleFunctionDefinition(const TSourceLoc&, TFunction&, const TAttributeMap&);
    void handleFunctionBody(const TSourceLoc&, TFunction&, TIntermNode* functionBody, TIntermNode*& node);
    void remapEntryPointIO(TFunction& function);
    void remapNonEntryPointIO(TFunction& function);
    TIntermNode* handleReturnValue(const TSourceLoc&, TIntermTyped*);
    void handleFunctionArgument(TFunction*, TIntermTyped*& arguments, TIntermTyped* newArg);
    TIntermTyped* handleAssign(const TSourceLoc&, TOperator, TIntermTyped* left, TIntermTyped* right) const;
    TIntermTyped* handleFunctionCall(const TSourceLoc&, TFunction*, TIntermNode*);
    void decomposeIntrinsic(const TSourceLoc&, TIntermTyped*& node, TIntermNode* arguments);
    void decomposeSampleMethods(const TSourceLoc&, TIntermTyped*& node, TIntermNode* arguments);
    void decomposeGeometryMethods(const TSourceLoc&, TIntermTyped*& node, TIntermNode* arguments);
    TIntermTyped* handleLengthMethod(const TSourceLoc&, TFunction*, TIntermNode*);
    void addInputArgumentConversions(const TFunction&, TIntermNode*&) const;
    TIntermTyped* addOutputArgumentConversions(const TFunction&, TIntermAggregate&);
    void builtInOpCheck(const TSourceLoc&, const TFunction&, TIntermOperator&);
    TFunction* handleConstructorCall(const TSourceLoc&, const TType&);
    void handleSemantic(TSourceLoc, TQualifier&, const TString& semantic);
    void handlePackOffset(const TSourceLoc&, TQualifier&, const glslang::TString& location,
                          const glslang::TString* component);
    void handleRegister(const TSourceLoc&, TQualifier&, const glslang::TString* profile, const glslang::TString& desc,
                        int subComponent, const glslang::TString*);

    TIntermAggregate* handleSamplerTextureCombine(const TSourceLoc& loc, TIntermTyped* argTex, TIntermTyped* argSampler);

    bool parseVectorFields(const TSourceLoc&, const TString&, int vecSize, TVectorFields&);
    void assignError(const TSourceLoc&, const char* op, TString left, TString right);
    void unaryOpError(const TSourceLoc&, const char* op, TString operand);
    void binaryOpError(const TSourceLoc&, const char* op, TString left, TString right);
    void variableCheck(TIntermTyped*& nodePtr);
    void constantValueCheck(TIntermTyped* node, const char* token);
    void integerCheck(const TIntermTyped* node, const char* token);
    void globalCheck(const TSourceLoc&, const char* token);
    bool constructorError(const TSourceLoc&, TIntermNode*, TFunction&, TOperator, TType&);
    bool constructorTextureSamplerError(const TSourceLoc&, const TFunction&);
    void arraySizeCheck(const TSourceLoc&, TIntermTyped* expr, TArraySize&);
    void arraySizeRequiredCheck(const TSourceLoc&, const TArraySizes&);
    void structArrayCheck(const TSourceLoc&, const TType& structure);
    void arrayDimMerge(TType& type, const TArraySizes* sizes);
    bool voidErrorCheck(const TSourceLoc&, const TString&, TBasicType);
    void boolCheck(const TSourceLoc&, const TIntermTyped*);
    void globalQualifierFix(const TSourceLoc&, TQualifier&);
    bool structQualifierErrorCheck(const TSourceLoc&, const TPublicType& pType);
    void mergeQualifiers(TQualifier& dst, const TQualifier& src);
    int computeSamplerTypeIndex(TSampler&);
    TSymbol* redeclareBuiltinVariable(const TSourceLoc&, const TString&, const TQualifier&, const TShaderQualifiers&);
    void redeclareBuiltinBlock(const TSourceLoc&, TTypeList& typeList, const TString& blockName, const TString* instanceName, TArraySizes* arraySizes);
    void paramFix(TType& type);
    void specializationCheck(const TSourceLoc&, const TType&, const char* op);

    void setLayoutQualifier(const TSourceLoc&, TQualifier&, TString&);
    void setLayoutQualifier(const TSourceLoc&, TQualifier&, TString&, const TIntermTyped*);
    void mergeObjectLayoutQualifiers(TQualifier& dest, const TQualifier& src, bool inheritOnly);
    void checkNoShaderLayouts(const TSourceLoc&, const TShaderQualifiers&);

    const TFunction* findFunction(const TSourceLoc& loc, const TFunction& call, bool& builtIn);
    void declareTypedef(const TSourceLoc&, TString& identifier, const TType&, TArraySizes* typeArray = 0);
    TIntermNode* declareVariable(const TSourceLoc&, TString& identifier, TType&, TIntermTyped* initializer = 0);
    TIntermTyped* addConstructor(const TSourceLoc&, TIntermNode*, const TType&);
    TIntermTyped* constructAggregate(TIntermNode*, const TType&, int, const TSourceLoc&);
    TIntermTyped* constructBuiltIn(const TType&, TOperator, TIntermTyped*, const TSourceLoc&, bool subset);
    void declareBlock(const TSourceLoc&, TType&, const TString* instanceName = 0, TArraySizes* arraySizes = 0);
    void finalizeGlobalUniformBlockLayout(TVariable& block);
    void fixBlockLocations(const TSourceLoc&, TQualifier&, TTypeList&, bool memberWithLocation, bool memberWithoutLocation);
    void fixBlockXfbOffsets(TQualifier&, TTypeList&);
    void fixBlockUniformOffsets(const TQualifier&, TTypeList&);
    void addQualifierToExisting(const TSourceLoc&, TQualifier, const TString& identifier);
    void addQualifierToExisting(const TSourceLoc&, TQualifier, TIdentifierList&);
    void updateStandaloneQualifierDefaults(const TSourceLoc&, const TPublicType&);
    void wrapupSwitchSubsequence(TIntermAggregate* statements, TIntermNode* branchNode);
    TIntermNode* addSwitch(const TSourceLoc&, TIntermTyped* expression, TIntermAggregate* body);

    void updateImplicitArraySize(const TSourceLoc&, TIntermNode*, int index);

    void nestLooping()       { ++loopNestingLevel; }
    void unnestLooping()     { --loopNestingLevel; }
    void nestAnnotations()   { ++annotationNestingLevel; }
    void unnestAnnotations() { --annotationNestingLevel; }
    int getAnnotationNestingLevel() { return annotationNestingLevel; }
    void pushScope()         { symbolTable.push(); }
    void popScope()          { symbolTable.pop(0); }

    void pushSwitchSequence(TIntermSequence* sequence) { switchSequenceStack.push_back(sequence); }
    void popSwitchSequence() { switchSequenceStack.pop_back(); }

    // Apply L-value conversions.  E.g, turning a write to a RWTexture into an ImageStore.
    TIntermTyped* handleLvalue(const TSourceLoc&, const char* op, TIntermTyped* node);
    bool lValueErrorCheck(const TSourceLoc&, const char* op, TIntermTyped*) override;

    TLayoutFormat getLayoutFromTxType(const TSourceLoc&, const TType&);

    bool handleOutputGeometry(const TSourceLoc&, const TLayoutGeometry& geometry);
    bool handleInputGeometry(const TSourceLoc&, const TLayoutGeometry& geometry);

protected:
    void inheritGlobalDefaults(TQualifier& dst) const;
    TVariable* makeInternalVariable(const char* name, const TType&) const;
    TVariable* declareNonArray(const TSourceLoc&, TString& identifier, TType&);
    void declareArray(const TSourceLoc&, TString& identifier, const TType&, TSymbol*&, bool track);
    TIntermNode* executeInitializer(const TSourceLoc&, TIntermTyped* initializer, TVariable* variable);
    TIntermTyped* convertInitializerList(const TSourceLoc&, const TType&, TIntermTyped* initializer);
    TOperator mapAtomicOp(const TSourceLoc& loc, TOperator op, bool isImage);

    // Return true if this node requires L-value conversion (e.g, to an imageStore).
    bool shouldConvertLValue(const TIntermNode*) const;

    // Array and struct flattening
    bool shouldFlatten(const TType& type) const { return shouldFlattenIO(type) || shouldFlattenUniform(type); }
    TIntermTyped* flattenAccess(TIntermTyped* base, int member);
    bool shouldFlattenIO(const TType&) const;
    bool shouldFlattenUniform(const TType&) const;
    void flatten(const TSourceLoc& loc, const TVariable& variable);
    void flattenStruct(const TVariable& variable);
    void flattenArray(const TSourceLoc& loc, const TVariable& variable);

    // Current state of parsing
    struct TPragma contextPragma;
    int loopNestingLevel;        // 0 if outside all loops
    int annotationNestingLevel;  // 0 if outside all annotations
    int structNestingLevel;      // 0 if outside blocks and structures
    int controlFlowNestingLevel; // 0 if outside all flow control
    TList<TIntermSequence*> switchSequenceStack;  // case, node, case, case, node, ...; ensure only one node between cases;   stack of them for nesting
    bool inEntryPoint;           // if inside a function, true if the function is the entry point
    bool postMainReturn;         // if inside a function, true if the function is the entry point and this is after a return statement
    const TType* currentFunctionType;  // the return type of the function that's currently being parsed
    bool functionReturnsValue;   // true if a non-void function has a return
    TBuiltInResource resources;
    TLimits& limits;

    HlslParseContext(HlslParseContext&);
    HlslParseContext& operator=(HlslParseContext&);

    static const int maxSamplerIndex = EsdNumDims * (EbtNumTypes * (2 * 2 * 2)); // see computeSamplerTypeIndex()
    TQualifier globalBufferDefaults;
    TQualifier globalUniformDefaults;
    TQualifier globalInputDefaults;
    TQualifier globalOutputDefaults;
    TString currentCaller;        // name of last function body entered (not valid when at global scope)
    TIdSetType inductiveLoopIds;
    TVector<TIntermTyped*> needsIndexLimitationChecking;
    TVariable* entryPointOutput;

    //
    // Geometry shader input arrays:
    //  - array sizing is based on input primitive and/or explicit size
    //
    // Tessellation control output arrays:
    //  - array sizing is based on output layout(vertices=...) and/or explicit size
    //
    // Both:
    //  - array sizing is retroactive
    //  - built-in block redeclarations interact with this
    //
    // Design:
    //  - use a per-context "resize-list", a list of symbols whose array sizes
    //    can be fixed
    //
    //  - the resize-list starts empty at beginning of user-shader compilation, it does
    //    not have built-ins in it
    //
    //  - on built-in array use: copyUp() symbol and add it to the resize-list
    //
    //  - on user array declaration: add it to the resize-list
    //
    //  - on block redeclaration: copyUp() symbol and add it to the resize-list
    //     * note, that appropriately gives an error if redeclaring a block that
    //       was already used and hence already copied-up
    //
    //  - on seeing a layout declaration that sizes the array, fix everything in the 
    //    resize-list, giving errors for mismatch
    //
    //  - on seeing an array size declaration, give errors on mismatch between it and previous
    //    array-sizing declarations
    //
    TVector<TSymbol*> ioArraySymbolResizeList;

    TMap<int, TVector<TVariable*>> flattenMap;
    unsigned int nextInLocation;
    unsigned int nextOutLocation;
};

} // end namespace glslang

#endif // HLSL_PARSE_INCLUDED_
