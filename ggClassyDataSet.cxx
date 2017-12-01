#include "ggClassyDataSet.h"


ggClassyDataSet* ggClassyDataSet::GenerateTestData()
{
  ggClassyClass* vClassA = new ggClassyClass();
  vClassA->mName = "ggClassA";
  vClassA->mMembers.push_back(ggClassyClassMember("Ping()", "void"));
  vClassA->mMembers.push_back(ggClassyClassMember("Pong()", "void"));
  vClassA->mMembers.push_back(ggClassyClassMember("GetB()", "ggClassB"));
  vClassA->mComment = "The answer to life the universe and everything.";

  ggClassyClass* vClassB = new ggClassyClass();
  vClassB->mName = "ggClassB";
  vClassB->mBaseClassNames.push_back("ggClassA");
  vClassB->mMembers.push_back(ggClassyClassMember("Blubb()", "QString"));
  vClassB->mMembers.push_back(ggClassyClassMember("GetA()", "ggClassA"));
  vClassB->mComment = "One fish, two fish, red fish, blue fish.";

  ggClassyClass* vClassC = new ggClassyClass();
  vClassC->mName = "ggClassC";
  vClassC->mBaseClassNames.push_back("ggClassA");
  vClassC->mBaseClassNames.push_back("ggClassB");
  vClassC->mMembers.push_back(ggClassyClassMember("GetName()", "QString"));
  vClassC->mMembers.push_back(ggClassyClassMember("SayHello()", "QString"));
  vClassC->mComment = "This is a very useful comment!";

  ggClassyClassBox* vClassBoxA1 = new ggClassyClassBox();
  vClassBoxA1->mClass = vClassA;
  vClassBoxA1->mPosition = QPointF(-300, -300);

  ggClassyClassBox* vClassBoxA2 = new ggClassyClassBox();
  vClassBoxA2->mClass = vClassA;
  vClassBoxA2->mPosition = QPointF(100, -300);

  ggClassyClassBox* vClassBoxB1 = new ggClassyClassBox();
  vClassBoxB1->mClass = vClassB;
  vClassBoxB1->mPosition = QPointF(-100, -100);
  vClassBoxB1->mMembersVisible = false;

  ggClassyClassBox* vClassBoxB2 = new ggClassyClassBox();
  vClassBoxB2->mClass = vClassB;
  vClassBoxB2->mPosition = QPointF(300, -100);
  vClassBoxB2->mMembersVisible = false;
  vClassBoxB2->mCommentVisible = false;

  ggClassyClassBox* vClassBoxC1 = new ggClassyClassBox();
  vClassBoxC1->mClass = vClassC;
  vClassBoxC1->mPosition = QPointF(-250, 0);
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
