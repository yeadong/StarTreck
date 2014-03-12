#include "GameScene.h"
#include "cocos-ext.h"

#include "NodeInit.h"
#include "PhysicsWorld.h"
#include "DebugLayer.h"
#include "StageManager.h"
#include "Camera.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define ACTOR_Z_ORDER 100
#define DEBUGLAYER_Z_ORDER 50

void GameScene::onEnter()
{
	CCScene::onEnter();
	CCLOG("------------------GameScene::onEnter()-----------------");

	//open update schedule
	scheduleUpdate();

	do 
	{
		//create Physics World
		CC_BREAK_IF(!PhysicsWorld::sharedPhysicsWorld()->init());

		DebugLayer* debugLayer = DebugLayer::create();
		CC_BREAK_IF(!debugLayer);
		addChild(debugLayer, DEBUGLAYER_Z_ORDER);
		//debugLayer->setVertexZ(1);

		//load scenenodes
		StageManager* stageMgr = StageManager::sharedStageManager();
		std::vector<StageInfo> stageData;
		CC_BREAK_IF(!stageMgr->readStageData(stageData));
		CCNode* pSceneRoot = stageMgr->loadStage(stageData[0].name);
		CC_BREAK_IF(!pSceneRoot);
		addChild(pSceneRoot, ACTOR_Z_ORDER);

		//init nodes
		CC_BREAK_IF(!NodeInitiator::sharedNodeInitiator()->initAllNodes(pSceneRoot));

		Camera* camera = Camera::sharedCamera();
		camera->setRootNode(pSceneRoot);
		camera->setFollowObjet(pSceneRoot->getChildByTag(10200));
		camera->setBoundingBox(CCSizeMake(200,200));

	} while (0);

}

void GameScene::onExit()
{
	CCScene::onExit();
	CCLOG("------------------GameScene::onExit()-----------------");
	unscheduleUpdate();
	
	Camera::sharedCamera()->reset();
	PhysicsWorld::sharedPhysicsWorld()->end();
}

void GameScene::update( float dt )
{
	Camera::sharedCamera()->update(dt);
	PhysicsWorld::sharedPhysicsWorld()->update(dt);
}


