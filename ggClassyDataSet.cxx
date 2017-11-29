#include "ggClassyDataSet.h"


ggClassyDataSet* ggClassyDataSet::GenerateTestData()
{
  ggClassyClass* vClassA = new ggClassyClass();
  vClassA->mName = "Class A";
  vClassA->mMembers.push_back(ggClassyClassMember("Ping()", "void"));
  vClassA->mMembers.push_back(ggClassyClassMember("Pong()", "bool"));
  vClassA->mComment = "Bli Bla Blu";

  ggClassyClass* vClassB = new ggClassyClass();
  vClassB->mName = "Class B";
  vClassB->mBaseClassNames.push_back("Class A");
  vClassB->mMembers.push_back(ggClassyClassMember("Blubb()", "QString"));
  vClassB->mComment = "One fish, two fish, red fish, blue fish.";

  ggClassyClass* vClassC = new ggClassyClass();
  vClassC->mName = "Class C";
  vClassC->mBaseClassNames.push_back("Class A");
  vClassC->mBaseClassNames.push_back("Class B");
  vClassC->mMembers.push_back(ggClassyClassMember("GetName()", "QString"));
  vClassC->mMembers.push_back(ggClassyClassMember("SayHello()", "Class A"));
  vClassC->mMembers.push_back(ggClassyClassMember("mName", "TheBase"));
  vClassC->mComment = "This is some very useful comment!";

  ggClassyClassBox* vClassBoxA1 = new ggClassyClassBox();
  vClassBoxA1->mClass = vClassA;
  vClassBoxA1->mPosition = QPointF(-300, -200);

  ggClassyClassBox* vClassBoxA2 = new ggClassyClassBox();
  vClassBoxA2->mClass = vClassA;
  vClassBoxA2->mPosition = QPointF(-150, -200);

  ggClassyClassBox* vClassBoxB1 = new ggClassyClassBox();
  vClassBoxB1->mClass = vClassB;
  vClassBoxB1->mPosition = QPointF(150, -200);
  vClassBoxB1->mMembersVisible = false;

  ggClassyClassBox* vClassBoxB2 = new ggClassyClassBox();
  vClassBoxB2->mClass = vClassB;
  vClassBoxB2->mPosition = QPointF(300, -200);
  vClassBoxB2->mMembersVisible = false;
  vClassBoxB2->mCommentVisible = false;

  ggClassyClassBox* vClassBoxC1 = new ggClassyClassBox();
  vClassBoxC1->mClass = vClassC;
  vClassBoxC1->mPosition = QPointF(-150, 0);
  vClassBoxC1->mWidth = 250.0f;

  ggClassyClassBox* vClassBoxC2 = new ggClassyClassBox();
  vClassBoxC2->mClass = vClassC;
  vClassBoxC2->mPosition = QPointF(150, 0);
  vClassBoxC2->mWidth = 150.0f;
  vClassBoxC2->mCommentVisible = false;

  ggClassyDataSet* vDataSet = new ggClassyDataSet();
  vDataSet->mClasses.insert(vClassA);
  vDataSet->mClasses.insert(vClassB);
  vDataSet->mClasses.insert(vClassC);
  vDataSet->mClassBoxes.push_back(vClassBoxA1);
  vDataSet->mClassBoxes.push_back(vClassBoxA2);
  vDataSet->mClassBoxes.push_back(vClassBoxB1);
  vDataSet->mClassBoxes.push_back(vClassBoxB2);
  vDataSet->mClassBoxes.push_back(vClassBoxC1);
  vDataSet->mClassBoxes.push_back(vClassBoxC2);

  return vDataSet;
}
