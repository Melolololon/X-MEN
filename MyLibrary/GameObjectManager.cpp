#include "GameObjectManager.h"
#include"CollisionDetectionData.h"
#include"LibMath.h"
#include"Collision.h"
#include"Physics.h"
#include"Values.h"
#include"Easing.h"


using namespace MelLib;

GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
}

GameObjectManager* GameObjectManager::GetInstance()
{
	static GameObjectManager inst;
	return &inst;
}

void GameObjectManager::Initialize()
{


	cursor = std::make_unique<MouseCursor>();
	//cursor->Initialize();

	addObjectSort = OBJECT_SORT_NONE;
	addObjectSortOrderType = false;

	addObjects.reserve(100);
}

void GameObjectManager::Update()
{


#pragma region オブジェクトのUpdate
	//カーソルアップデート
	if (cursor)
	{
		cursor->Update();
		nearPos = cursor->GetNearPos();
		farPos = cursor->GetFarPos();
	}

	for (auto& obj : objects)
	{
		obj->SetPreDataPositions();
		obj->Update();

		// 仮にここに書いてる
		obj->SetPreDataPositions();
	}

	for (auto& obj : object2Ds)
	{
		obj->Update();

	}

	if (addObjects.size() != 0)
	{
		for (auto& a : addObjects)
		{
			a.get()->SetPreDataPositions();
			a.get()->Update();
			objects.push_back(a);

			// 仮にここに書いてる
			a->SetPreDataPositions();
		}

		if (addObjectSort != OBJECT_SORT_NONE)ObjectSort(addObjectSort, addObjectSortOrderType);

		addObjects.clear();
	}


	if (addObject2Ds.size() != 0)
	{
		for (auto& a : addObject2Ds)
		{

			a.get()->Update();
			object2Ds.push_back(a);
		}

		addObject2Ds.clear();
	}

#pragma endregion

#pragma region 新判定処理

	size_t objectSize = objects.size();

	std::vector<CollisionDetectionFlag>collisionFlags(objectSize);
	for (int i = 0; i < objectSize; i++)
	{
		collisionFlags[i] = objects[i]->GetCollisionFlag();
	}

	for (int objI = 0; objI < objectSize; objI++)
	{
		GameObject* obj1 = objects[objI].get();
		for (int objJ = 0; objJ < objectSize; objJ++)
		{
			GameObject* obj2 = objects[objJ].get();

			////自分と比較、比較済の組み合わせはcontinue
			if (objI >= objJ)continue;

			unsigned int checkNum = 1;
			auto getCheckNum = [](const GameObject& obj1, ShapeType3D type1, const GameObject& obj2, ShapeType3D type2)
			{
				unsigned int num1 = obj1.GetFrameHitCheckNumber(type1);
				unsigned int num2 = obj2.GetFrameHitCheckNumber(type2);
				if (num1 > num2)return num1;
				return num2;
			};

			// 応急処置
			float checkDistance = obj1->GetCollisionCheckDistance();
			if (checkDistance < obj2->GetCollisionCheckDistance())checkDistance = obj2->GetCollisionCheckDistance();

			float distance = LibMath::CalcDistance3D(obj1->GetPosition(), obj2->GetPosition());
			if (distance > checkDistance)continue;

#pragma region Sphere & Sphere


			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].sphere)
			{

				std::unordered_map<std::string, std::vector<SphereData>> sphereDatas1 = obj1->GetSphereDatas();
				std::unordered_map<std::string, std::vector<SphereData>> sphereDatas2 = obj2->GetSphereDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreSpherePositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreSpherePositions(prePositions2);

				// 名前分ループ
				for (const auto& sphereData1 : sphereDatas1)
				{
					for (const auto& sphereData2 : sphereDatas2)
					{
						std::vector<SphereData>sphereDataVec1 = sphereDatas1[sphereData1.first];
						size_t sphereData1Size = sphereDataVec1.size();
						std::vector<SphereData>sphereDataVec2 = sphereDatas2[sphereData1.first];
						size_t sphereData2Size = sphereDataVec2.size();


						for (int colI = 0; colI < sphereData1Size; colI++)
						{
							for (int colJ = 0; colJ < sphereData2Size; colJ++)
							{

								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj1, ShapeType3D::SPHERE, *obj2, ShapeType3D::SPHERE);

								SphereData sphere1 = sphereDataVec1[colI];
								SphereData sphere2 = sphereDataVec2[colJ];

								// 座標を補完
								Vector3 pos1 = sphere1.GetPosition();
								Vector3 prePos1 = prePositions1[sphereData1.first][colI];
								Vector3 pos2 = sphere2.GetPosition();
								Vector3 prePos2 = prePositions2[sphereData2.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));

								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();

								for (int c = 0; c < checkNum; c++)
								{
									sphere1.SetPosition(easing1.PreLerp());
									sphere2.SetPosition(easing2.PreLerp());

									if (Collision::SphereAndSphere(sphere1, sphere2))
									{
										//hitを呼び出す
										obj1->SetHitSphereData(sphere2);
										obj2->SetHitSphereData(sphere1);

										// オブジェクトに補間した座標をセット
										// セットしたくないのにセットされたらいやだから
										// 各自取得するようにする
										obj1->SetLerpPosition(sphere1.GetPosition());
										obj2->SetLerpPosition(sphere2.GetPosition());

										obj1->SetLerpMovePosition(easingMovePos1);
										obj2->SetLerpMovePosition(easingMovePos2);

										obj1->Hit
										(
											*obj2,
											ShapeType3D::SPHERE,
											sphereData1.first,
											ShapeType3D::SPHERE,
											sphereData2.first
										);
										obj2->Hit
										(
											*obj1,
											ShapeType3D::SPHERE,
											sphereData2.first,
											ShapeType3D::SPHERE,
											sphereData1.first
										);

										break;
									}

								}
							}
						}
					}
				}

			}
#pragma endregion

#pragma region Box & Box
			if (collisionFlags[objI].box
				&& collisionFlags[objJ].box)
			{

				std::unordered_map < std::string, std::vector<BoxData>>boxDatas1 = obj1->GetBoxDatas();

				std::unordered_map < std::string, std::vector<BoxData>>boxDatas2 = obj2->GetBoxDatas();


				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreBoxPositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreBoxPositions(prePositions2);


				// 名前分ループ
				for (const auto& boxData1 : boxDatas1)
				{
					for (const auto& boxData2 : boxDatas2)
					{
						std::vector<BoxData>boxDataVec1 = boxData1.second;
						std::vector<BoxData>boxDataVec2 = boxData2.second;

						size_t boxData1Size = boxDataVec1.size();
						size_t boxData2Size = boxDataVec2.size();

						for (int colI = 0; colI < boxData1Size; colI++)
						{
							for (int colJ = 0; colJ < boxData2Size; colJ++)
							{
								BoxCalcResult result1;
								BoxCalcResult result2;

								if (Collision::BoxAndBox(boxDataVec1[colI], &result1, boxDataVec2[colJ], &result2))
								{
									obj1->SetBoxCalcResult(result1);
									obj2->SetBoxCalcResult(result2);

									obj1->SetHitBoxData(boxDataVec2[colJ]);
									obj2->SetHitBoxData(boxDataVec1[colI]);

									//hitを呼び出す
									obj1->Hit
									(
										*obj2,
										ShapeType3D::BOX,
										boxData1.first,
										ShapeType3D::BOX,
										boxData2.first
									);
									obj2->Hit
									(
										*obj1,
										ShapeType3D::BOX,
										boxData2.first,
										ShapeType3D::BOX,
										boxData1.first
									);
								}
							}
						}
					}
				}

			}
#pragma endregion

#pragma region Segment & Segment
			if (collisionFlags[objI].segment
				&& collisionFlags[objJ].segment)
			{
				std::unordered_map < std::string, std::vector<Segment3DData>>segmentDatas1 = obj1->GetSegmentDatas();
				std::unordered_map < std::string, std::vector<Segment3DData>>segmentDatas2 = obj2->GetSegmentDatas();

				// 名前分ループ
				for (const auto& segmentData1 : segmentDatas1)
				{
					for (const auto& segmentData2 : segmentDatas2)
					{
						std::vector<Segment3DData>segmentDataVec1 = segmentData1.second;
						size_t segmentData1Size = segmentDataVec1.size();
						std::vector<Segment3DData>segmentDataVec2 = segmentData2.second;
						size_t segmentData2Size = segmentDataVec2.size();


						for (int colI = 0; colI < segmentData1Size; colI++)
						{
							for (int colJ = 0; colJ < segmentData2Size; colJ++)
							{
								Segment3DCalcResult result1;
								Segment3DCalcResult result2;


								if (Collision::Segment3DAndSegment3D
								(
									segmentDataVec1[colI],
									&result1,
									segmentDataVec2[colJ],
									&result2
								))
								{
									obj1->SetSegmentCalcResult(result1);
									obj2->SetSegmentCalcResult(result2);


									obj1->SetHitSegment3DData(segmentDataVec2[colJ]);
									obj2->SetHitSegment3DData(segmentDataVec1[colI]);

									//hitを呼び出す
									obj1->Hit
									(
										*obj2,
										ShapeType3D::SEGMENT,
										segmentData1.first,
										ShapeType3D::SEGMENT,
										segmentData2.first
									);
									obj2->Hit
									(
										*obj1,
										ShapeType3D::SEGMENT,
										segmentData2.first,
										ShapeType3D::SEGMENT,
										segmentData2.first
									);
								}
							}
						}
					}
				}

			}
#pragma endregion

#pragma region Capsule & Capsule
			if (collisionFlags[objI].capsule
				&& collisionFlags[objJ].capsule)
			{
				std::unordered_map < std::string, std::vector<CapsuleData>>capsuleDatas1 = obj1->GetCapsuleDatas();
				std::unordered_map < std::string, std::vector<CapsuleData>>capsuleDatas2 = obj2->GetCapsuleDatas();

				// 名前分ループ
				for (const auto& capsuleData1 : capsuleDatas1)
				{
					for (const auto& capsuleData2 : capsuleDatas2)
					{

						std::vector<CapsuleData>capsuleDataVec1 = capsuleData1.second;
						size_t capsuleDataVec1Size = capsuleDataVec1.size();
						std::vector<CapsuleData>capsuleDataVec2 = capsuleData2.second;
						size_t capsuleDataVec2Size = capsuleDataVec2.size();


						for (int colI = 0; colI < capsuleDataVec1Size; colI++)
						{
							for (int colJ = 0; colJ < capsuleDataVec2Size; colJ++)
							{
								if (Collision::CapsuleAndCapsule(capsuleDataVec1[colI], capsuleDataVec2[colJ]))
								{
									obj1->SetHitCapsuleData(capsuleDataVec2[colJ]);
									obj2->SetHitCapsuleData(capsuleDataVec1[colI]);


									//hitを呼び出す
									obj1->Hit
									(
										*obj2,
										ShapeType3D::CAPSULE,
										capsuleData1.first,
										ShapeType3D::CAPSULE,
										capsuleData2.first
									);
									obj2->Hit
									(
										*obj1,
										ShapeType3D::CAPSULE,
										capsuleData2.first,
										ShapeType3D::CAPSULE,
										capsuleData1.first
									);
								}
							}
						}
					}
				}
			}
#pragma endregion

#pragma region Sphere & Box
			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].box)
			{

				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj1->GetSphereDatas();
				std::unordered_map < std::string, std::vector<BoxData>>boxDatas = obj2->GetBoxDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreSpherePositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreBoxPositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& boxData : boxDatas)
					{
						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<BoxData>boxDataVec = boxData.second;
						size_t boxDataSize = boxDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < boxDataSize; colJ++)
							{
								SphereCalcResult result1;
								BoxCalcResult result2;



								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj1, ShapeType3D::SPHERE, *obj2, ShapeType3D::BOX);

								SphereData sphere1 = sphereDataVec[colI];
								BoxData box = boxDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = sphere1.GetPosition();
								Vector3 prePos1 = prePositions1[sphereData.first][colI];
								Vector3 pos2 = box.GetPosition();
								Vector3 prePos2 = prePositions2[boxData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));

								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									sphere1.SetPosition(easing1.PreLerp());
									box.SetPosition(easing2.PreLerp());


									if (Collision::SphereAndBox
									(
										sphere1,
										&result1,
										box,
										&result2
									))
									{
										obj1->SetSphereCalcResult(result1);
										obj2->SetBoxCalcResult(result2);

										obj1->SetHitBoxData(box);
										obj2->SetHitSphereData(sphere1);

										obj1->SetLerpPosition(sphere1.GetPosition());
										obj2->SetLerpPosition(box.GetPosition());

										obj1->SetLerpMovePosition(easingMovePos1);
										obj2->SetLerpMovePosition(easingMovePos2);

										//hitを呼び出す
										obj1->Hit
										(
											*obj2,
											ShapeType3D::SPHERE,
											sphereData.first,
											ShapeType3D::BOX,
											boxData.first
										);
										obj2->Hit
										(
											*obj1,
											ShapeType3D::BOX,
											boxData.first,
											ShapeType3D::SPHERE,
											sphereData.first
										);
										break;
									}
								}
							}

						}
					}


				}
			}


			if (collisionFlags[objJ].sphere
				&& collisionFlags[objI].box)
			{
				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj2->GetSphereDatas();
				std::unordered_map < std::string, std::vector<BoxData>>boxDatas = obj1->GetBoxDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreBoxPositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreSpherePositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& boxData : boxDatas)
					{

						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<BoxData>boxDataVec = boxData.second;
						size_t boxDataSize = boxDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < boxDataSize; colJ++)
							{
								SphereCalcResult result1;
								BoxCalcResult result2;

								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj2, ShapeType3D::SPHERE, *obj1, ShapeType3D::BOX);

								SphereData sphere1 = sphereDataVec[colI];
								BoxData box = boxDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = box.GetPosition();
								Vector3 prePos1 = prePositions1[boxData.first][colI];
								Vector3 pos2 = sphere1.GetPosition();
								Vector3 prePos2 = prePositions2[sphereData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));
								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									sphere1.SetPosition(easing2.PreLerp());
									box.SetPosition(easing1.PreLerp());

									if (Collision::SphereAndBox
									(
										sphere1,
										&result1,
										box,
										&result2
									))
									{
										obj2->SetSphereCalcResult(result1);
										obj1->SetBoxCalcResult(result2);

										obj2->SetHitSphereData(sphere1);
										obj1->SetHitBoxData(box);

										obj2->SetLerpPosition(sphere1.GetPosition());
										obj1->SetLerpPosition(box.GetPosition());

										obj2->SetLerpMovePosition(easingMovePos2);
										obj1->SetLerpMovePosition(easingMovePos1);
										//hitを呼び出す
										obj2->Hit
										(
											*obj1,
											ShapeType3D::SPHERE,
											sphereData.first,
											ShapeType3D::BOX,
											boxData.first
										);
										obj1->Hit
										(
											*obj2,
											ShapeType3D::BOX,
											boxData.first,
											ShapeType3D::SPHERE,
											sphereData.first
										);
										break;
									}
								}
							}
						}
					}
				}

			}

#pragma endregion

#pragma region sphere & OBB
			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].obb)
			{

				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj1->GetSphereDatas();
				std::unordered_map < std::string, std::vector<OBBData>>obbDatas = obj2->GetOBBDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreSpherePositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreOBBPositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& obbData : obbDatas)
					{
						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<OBBData>obbDataVec = obbData.second;
						size_t obbDataSize = obbDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < obbDataSize; colJ++)
							{

								SphereCalcResult result1;
								//BoxCalcResult result2;



								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj1, ShapeType3D::SPHERE, *obj2, ShapeType3D::SPHERE);

								SphereData sphere1 = sphereDataVec[colI];
								OBBData obb = obbDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = sphere1.GetPosition();
								Vector3 prePos1 = prePositions1[sphereData.first][colI];
								Vector3 pos2 = obb.GetPosition();
								Vector3 prePos2 = prePositions2[obbData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));
								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									sphere1.SetPosition(easing1.PreLerp());
									obb.SetPosition(easing2.PreLerp());


									if (Collision::SphereAndOBB
									(
										sphere1,
										&result1,
										obb/*,
										&result2*/
									))
									{
										obj1->SetSphereCalcResult(result1);
										//obj2->SetBoxCalcResult(result2);

										obj1->SetHitOBBData(obb);
										obj2->SetHitSphereData(sphere1);

										obj1->SetLerpPosition(sphere1.GetPosition());
										obj2->SetLerpPosition(obb.GetPosition());

										obj1->SetLerpMovePosition(easingMovePos1);
										obj2->SetLerpMovePosition(easingMovePos2);

										//hitを呼び出す
										obj1->Hit
										(
											*obj2,
											ShapeType3D::SPHERE,
											sphereData.first,
											ShapeType3D::OBB,
											obbData.first
										);
										obj2->Hit
										(
											*obj1,
											ShapeType3D::OBB,
											obbData.first,
											ShapeType3D::SPHERE,
											sphereData.first
										);
										break;
									}
								}
							}

						}
					}


				}
			}

			if (collisionFlags[objJ].sphere
				&& collisionFlags[objI].obb)
			{
				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj2->GetSphereDatas();
				std::unordered_map < std::string, std::vector<OBBData>>obbDatas = obj1->GetOBBDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreOBBPositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreSpherePositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& obbData : obbDatas)
					{

						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<OBBData>obbDataVec = obbData.second;
						size_t obbDataSize = obbDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < obbDataSize; colJ++)
							{
								SphereCalcResult result1;
								//BoxCalcResult result2;
									// 判定を行う回数を取得
								checkNum = getCheckNum(*obj2, ShapeType3D::SPHERE, *obj1, ShapeType3D::SPHERE);

								SphereData sphere1 = sphereDataVec[colI];
								OBBData obb = obbDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = obb.GetPosition();
								Vector3 prePos1 = prePositions1[obbData.first][colI];
								Vector3 pos2 = sphere1.GetPosition();
								Vector3 prePos2 = prePositions2[sphereData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));
								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									obb.SetPosition(easing1.PreLerp());
									sphere1.SetPosition(easing2.PreLerp());

									if (Collision::SphereAndOBB
									(
										sphere1,
										&result1,
										obb/*,
										&result2*/
									))
									{
										obj2->SetSphereCalcResult(result1);
										//obj1->SetBoxCalcResult(result2);

										obj1->SetHitSphereData(sphere1);
										obj2->SetHitOBBData(obb);

										obj2->SetLerpPosition(sphere1.GetPosition());
										obj1->SetLerpPosition(obb.GetPosition());

										obj2->SetLerpMovePosition(easingMovePos2);
										obj1->SetLerpMovePosition(easingMovePos1);
										//hitを呼び出す
										obj2->Hit
										(
											*obj1,
											ShapeType3D::OBB,
											sphereData.first,
											ShapeType3D::SPHERE,
											obbData.first
										);
										obj1->Hit
										(
											*obj2,
											ShapeType3D::SPHERE,
											obbData.first,
											ShapeType3D::OBB,
											sphereData.first
										);
										break;
									}
								}
							}
						}
					}
				}

			}

#pragma endregion

#pragma region Sphere & Triangle
			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].triangle)
			{

				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj1->GetSphereDatas();
				std::unordered_map < std::string, std::vector<TriangleData>>triangleDatas = obj2->GetTriangleDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreSpherePositions(prePositions1);
				std::unordered_map<std::string, std::vector<Value3<Vector3>>>prePositions2;
				obj2->GetPreTrianglePositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& triangleData : triangleDatas)
					{
						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<TriangleData>triangleDataVec = triangleData.second;
						size_t triangleDataSize = triangleDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < triangleDataSize; colJ++)
							{
								SphereCalcResult result1;
								TriangleCalcResult result2;



								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj1, ShapeType3D::SPHERE, *obj2, ShapeType3D::TRIANGLE);

								SphereData sphere1 = sphereDataVec[colI];
								TriangleData triangle = triangleDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = sphere1.GetPosition();
								Vector3 prePos1 = prePositions1[sphereData.first][colI];
								Value3< Vector3> pos2 = triangle.GetPosition();
								Value3< Vector3>prePos2 = prePositions2[triangleData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Value3<Easing<Vector3>>easing2;
								easing2.v1 = Easing<Vector3>(prePos2.v1, pos2.v1, 100.0f / static_cast<float>(checkNum));
								easing2.v2 = Easing<Vector3>(prePos2.v2, pos2.v2, 100.0f / static_cast<float>(checkNum));
								easing2.v3 = Easing<Vector3>(prePos2.v3, pos2.v3, 100.0f / static_cast<float>(checkNum));

								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Value3<Vector3> easingMovePos2;
								easingMovePos2.v1 = easing2.v1.GetFrameLarpValue();
								easingMovePos2.v2 = easing2.v2.GetFrameLarpValue();
								easingMovePos2.v3 = easing2.v3.GetFrameLarpValue();

								for (int c = 0; c < checkNum; c++)
								{
									sphere1.SetPosition(easing1.PreLerp());

									Value3<Vector3> tPos = Value3<Vector3>(easing2.v1.PreLerp(), easing2.v2.PreLerp(), easing2.v3.PreLerp());
									triangle.SetPosition(tPos);


									if (Collision::SphereAndTriangle
									(
										sphere1,
										triangle
									))
									{
										obj1->SetSphereCalcResult(result1);
										obj2->SetTriangleCalcResult(result2);

										obj1->SetHitTriangleData(triangle);
										obj2->SetHitSphereData(sphere1);

										obj1->SetLerpPosition(sphere1.GetPosition());
										//obj2->SetLerpPosition(triangle.GetPosition());

										obj1->SetLerpMovePosition(easingMovePos1);
										//obj2->SetLerpMovePosition(easingMovePos2);

										//hitを呼び出す
										obj1->Hit
										(
											*obj2,
											ShapeType3D::SPHERE,
											sphereData.first,
											ShapeType3D::TRIANGLE,
											triangleData.first
										);
										obj2->Hit
										(
											*obj1,
											ShapeType3D::TRIANGLE,
											triangleData.first,
											ShapeType3D::SPHERE,
											sphereData.first
										);
										break;
									}
								}
							}

						}
					}


				}
			}


			if (collisionFlags[objJ].sphere
				&& collisionFlags[objI].triangle)
			{
				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj2->GetSphereDatas();
				std::unordered_map < std::string, std::vector<TriangleData>>triangleDatas = obj1->GetTriangleDatas();

				std::unordered_map<std::string, std::vector<Value3<Vector3>>>prePositions1;
				obj1->GetPreTrianglePositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreSpherePositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& triangleData : triangleDatas)
					{

						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<TriangleData>triangleDataVec = triangleData.second;
						size_t triangleDataSize = triangleDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < triangleDataSize; colJ++)
							{
								SphereCalcResult result1;
								TriangleCalcResult result2;

								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj2, ShapeType3D::SPHERE, *obj1, ShapeType3D::TRIANGLE);

								SphereData sphere1 = sphereDataVec[colI];
								TriangleData triangle = triangleDataVec[colJ];

								// 座標を補完
								Value3< Vector3> pos1 = triangle.GetPosition();
								Value3< Vector3> prePos1 = prePositions1[triangleData.first][colI];
								Vector3 pos2 = sphere1.GetPosition();
								Vector3 prePos2 = prePositions2[sphereData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Value3<Easing<Vector3>>easing1;
								easing1.v1 = Easing<Vector3>(prePos1.v1, pos1.v1, 100.0f / static_cast<float>(checkNum));
								easing1.v2 = Easing<Vector3>(prePos1.v2, pos1.v2, 100.0f / static_cast<float>(checkNum));
								easing1.v3 = Easing<Vector3>(prePos1.v3, pos1.v3, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));

								Value3<Vector3> easingMovePos1;
								easingMovePos1.v1 = easing1.v1.GetFrameLarpValue();
								easingMovePos1.v2 = easing1.v2.GetFrameLarpValue();
								easingMovePos1.v3 = easing1.v3.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();

								for (int c = 0; c < checkNum; c++)
								{
									sphere1.SetPosition(easing2.PreLerp());

									Value3<Vector3> tPos = Value3<Vector3>(easing1.v1.PreLerp(), easing1.v2.PreLerp(), easing1.v3.PreLerp());
									triangle.SetPosition(tPos);

									if (Collision::SphereAndTriangle
									(
										sphere1,
										triangle
									))
									{
										obj2->SetSphereCalcResult(result1);
										obj1->SetTriangleCalcResult(result2);

										obj2->SetHitSphereData(sphere1);
										obj1->SetHitTriangleData(triangle);

										obj2->SetLerpPosition(sphere1.GetPosition());
										//obj1->SetLerpPosition(triangle.GetPosition());

										obj2->SetLerpMovePosition(easingMovePos2);
										//obj1->SetLerpMovePosition(easingMovePos1);
										//hitを呼び出す
										obj2->Hit
										(
											*obj1,
											ShapeType3D::SPHERE,
											sphereData.first,
											ShapeType3D::TRIANGLE,
											triangleData.first
										);
										obj1->Hit
										(
											*obj2,
											ShapeType3D::TRIANGLE,
											triangleData.first,
											ShapeType3D::SPHERE,
											sphereData.first
										);
										break;
									}
								}
							}
						}
					}
				}

			}

#pragma endregion

#pragma region Sphere & Capsule
			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].capsule)
			{
				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj1->GetSphereDatas();
				std::unordered_map < std::string, std::vector<CapsuleData>>capsuleDatas = obj2->GetCapsuleDatas();

				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& capsuleData : capsuleDatas)
					{
						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<CapsuleData>capsuleDataVec = capsuleData.second;
						size_t capsuleDataSize = capsuleDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < capsuleDataSize; colJ++)
							{
								if (Collision::SphereAndCapsule(sphereDataVec[colI], capsuleDataVec[colJ]))
								{
									obj1->SetHitCapsuleData(capsuleDataVec[colJ]);
									obj2->SetHitSphereData(sphereDataVec[colI]);


									//hitを呼び出す
									obj1->Hit
									(
										*obj2,
										ShapeType3D::SPHERE,
										sphereData.first,
										ShapeType3D::CAPSULE,
										capsuleData.first
									);
									obj2->Hit
									(
										*obj1,
										ShapeType3D::CAPSULE,
										capsuleData.first,
										ShapeType3D::SPHERE,
										sphereData.first
									);

								}
							}
						}
					}
				}

			}

			if (collisionFlags[objJ].sphere
				&& collisionFlags[objI].capsule)
			{
				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj2->GetSphereDatas();
				std::unordered_map < std::string, std::vector<CapsuleData>>capsuleDatas = obj1->GetCapsuleDatas();

				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& capsuleData : capsuleDatas)
					{
						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<CapsuleData>capsuleDataVec = capsuleData.second;
						size_t capsuleDataSize = capsuleDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < capsuleDataSize; colJ++)
							{
								if (Collision::SphereAndCapsule(sphereDataVec[colI], capsuleDataVec[colJ]))
								{
									obj1->SetHitSphereData(sphereDataVec[colI]);
									obj2->SetHitCapsuleData(capsuleDataVec[colJ]);

									//hitを呼び出す
									obj2->Hit
									(
										*obj1,
										ShapeType3D::SPHERE,
										sphereData.first,
										ShapeType3D::CAPSULE,
										capsuleData.first
									);
									obj1->Hit
									(
										*obj2,
										ShapeType3D::CAPSULE,
										capsuleData.first,
										ShapeType3D::SPHERE,
										sphereData.first
									);

								}
							}
						}
					}
				}
			}

#pragma endregion

#pragma region Board & Segent
			if (collisionFlags[objI].board
				&& collisionFlags[objJ].segment)
			{
				std::unordered_map < std::string, std::vector<BoardData>>boardDatas = obj1->GetBoardDatas();
				std::unordered_map < std::string, std::vector<Segment3DData>>segmentDatas = obj2->GetSegmentDatas();

				// 名前分ループ
				for (const auto& boardData : boardDatas)
				{
					for (const auto& segmentData : segmentDatas)
					{
						std::vector<BoardData>boardDataVec = boardData.second;
						size_t boardDataSize = boardDataVec.size();
						std::vector<Segment3DData>segmentDataVec = segmentData.second;
						size_t segmentDataSize = segmentDataVec.size();

						for (int colI = 0; colI < boardDataSize; colI++)
						{
							for (int colJ = 0; colJ < segmentDataSize; colJ++)
							{
								BoardCalcResult result1;
								Segment3DCalcResult result2;

								if (Collision::BoardAndSegment3D(boardDataVec[colI], &result1, segmentDataVec[colJ], &result2))
								{
									obj1->SetBoardCalcResult(result1);
									obj2->SetSegmentCalcResult(result2);

									obj1->SetHitSegment3DData(segmentDataVec[colJ]);
									obj2->SetHitBoardData(boardDataVec[colI]);

									//hitを呼び出す
									obj1->Hit
									(
										*obj2,
										ShapeType3D::BOARD,
										boardData.first,
										ShapeType3D::SEGMENT,
										segmentData.first
									);
									obj2->Hit
									(
										*obj1,
										ShapeType3D::SEGMENT,
										segmentData.first,
										ShapeType3D::BOARD,
										boardData.first
									);

								}
							}
						}

					}
				}
			}

			if (collisionFlags[objI].board
				&& collisionFlags[objJ].segment)
			{
				std::unordered_map < std::string, std::vector<BoardData>>boardDatas = obj2->GetBoardDatas();
				std::unordered_map < std::string, std::vector<Segment3DData>>segmentDatas = obj1->GetSegmentDatas();
				// 名前分ループ
				for (const auto& boardData : boardDatas)
				{
					for (const auto& segmentData : segmentDatas)
					{

						std::vector<BoardData>boardDataVec = boardData.second;
						size_t boardDataSize = boardDataVec.size();
						std::vector<Segment3DData>segmentDataVec = segmentData.second;
						size_t segmentDataSize = segmentDataVec.size();

						for (int colI = 0; colI < boardDataSize; colI++)
						{
							for (int colJ = 0; colJ < segmentDataSize; colJ++)
							{
								BoardCalcResult result1;
								Segment3DCalcResult result2;

								if (Collision::BoardAndSegment3D(boardDataVec[colI], &result1, segmentDataVec[colJ], &result2))
								{
									obj2->SetBoardCalcResult(result1);
									obj1->SetSegmentCalcResult(result2);

									obj1->SetHitBoardData(boardDataVec[colI]);
									obj2->SetHitSegment3DData(segmentDataVec[colJ]);

									//hitを呼び出す
									obj2->Hit
									(
										*obj1,
										ShapeType3D::BOARD,
										boardData.first,
										ShapeType3D::SEGMENT,
										segmentData.first
									);
									obj1->Hit
									(
										*obj2,
										ShapeType3D::SEGMENT,
										segmentData.first,
										ShapeType3D::BOARD,
										boardData.first
									);

								}
							}
						}
					}
				}

			}

#pragma endregion

#pragma region Board & Capsule
			if (collisionFlags[objI].board
				&& collisionFlags[objJ].capsule)
			{
				std::unordered_map < std::string, std::vector<BoardData>>boardDatas = obj1->GetBoardDatas();
				std::unordered_map < std::string, std::vector<CapsuleData>>capsuleDatas = obj2->GetCapsuleDatas();

				// 名前分ループ
				for (const auto& boardData : boardDatas)
				{
					for (const auto& capsuleData : capsuleDatas)
					{
						std::vector<BoardData>boardDataVec = boardData.second;
						size_t boardDataSize = boardDataVec.size();
						std::vector<CapsuleData>capsuleDataVec = capsuleData.second;
						size_t capsuleDataSize = capsuleDataVec.size();

						for (int colI = 0; colI < boardDataSize; colI++)
						{
							for (int colJ = 0; colJ < capsuleDataSize; colJ++)
							{
								BoardCalcResult result1;
								Segment3DCalcResult result2;

								if (Collision::BoardAndCapsule(boardDataVec[colI], &result1, capsuleDataVec[colJ], &result2))
								{
									obj1->SetBoardCalcResult(result1);
									obj2->SetCapsuleCalcResult(result2);

									obj1->SetHitCapsuleData(capsuleDataVec[colJ]);
									obj2->SetHitBoardData(boardDataVec[colI]);

									//hitを呼び出す
									obj1->Hit
									(
										*obj2,
										ShapeType3D::BOARD,
										boardData.first,
										ShapeType3D::CAPSULE,
										capsuleData.first
									);
									obj2->Hit
									(
										*obj1,
										ShapeType3D::CAPSULE,
										capsuleData.first,
										ShapeType3D::BOARD,
										boardData.first
									);

								}
							}
						}
					}
				}

			}

			if (collisionFlags[objJ].board
				&& collisionFlags[objI].capsule)
			{
				std::unordered_map < std::string, std::vector<BoardData>>boardDatas = obj2->GetBoardDatas();
				std::unordered_map < std::string, std::vector<CapsuleData>>capsuleDatas = obj1->GetCapsuleDatas();

				// 名前分ループ
				for (const auto& boardData : boardDatas)
				{
					for (const auto& capsuleData : capsuleDatas)
					{
						std::vector<BoardData>boardDataVec = boardData.second;
						size_t boardDataSize = boardDataVec.size();
						std::vector<CapsuleData>capsuleDataVec = capsuleData.second;
						size_t capsuleDataSize = capsuleDataVec.size();

						for (int colI = 0; colI < boardDataSize; colI++)
						{
							for (int colJ = 0; colJ < capsuleDataSize; colJ++)
							{
								BoardCalcResult result1;
								Segment3DCalcResult result2;

								if (Collision::BoardAndCapsule(boardDataVec[colI], &result1, capsuleDataVec[colJ], &result2))
								{
									obj2->SetBoardCalcResult(result1);
									obj1->SetCapsuleCalcResult(result2);

									obj1->SetHitBoardData(boardDataVec[colI]);
									obj2->SetHitCapsuleData(capsuleDataVec[colJ]);

									//hitを呼び出す
									obj2->Hit
									(
										*obj1,
										ShapeType3D::BOARD,
										boardData.first,
										ShapeType3D::CAPSULE,
										capsuleData.first
									);
									obj1->Hit
									(
										*obj2,
										ShapeType3D::CAPSULE,
										capsuleData.first,
										ShapeType3D::BOARD,
										boardData.first
									);

								}
							}
						}

					}
				}
			}
#pragma endregion

#pragma region Triangle & Segment
			if (collisionFlags[objI].triangle
				&& collisionFlags[objJ].segment)
			{

				std::unordered_map < std::string, std::vector<TriangleData>>triangleDatas = obj1->GetTriangleDatas();
				std::unordered_map < std::string, std::vector<Segment3DData>>segmentDatas = obj2->GetSegmentDatas();

				// 名前分ループ
				for (const auto& triangleData : triangleDatas)
				{
					for (const auto& segmentData : segmentDatas)
					{

						std::vector<TriangleData>triangleDataVec = triangleData.second;
						size_t treiangleDataSize = triangleDataVec.size();
						std::vector<Segment3DData>segmentDataVec = segmentData.second;
						size_t segmentDataSize = segmentDataVec.size();

						for (int colI = 0; colI < treiangleDataSize; colI++)
						{
							for (int colJ = 0; colJ < segmentDataSize; colJ++)
							{
								TriangleCalcResult result1;
								Segment3DCalcResult result2;

								if (Collision::TriangleAndSegment3D(triangleDataVec[colI], &result1, segmentDataVec[colJ], &result2))
								{
									obj1->SetTriangleCalcResult(result1);
									obj2->SetSegmentCalcResult(result2);

									obj1->SetHitSegment3DData(segmentDataVec[colJ]);
									obj2->SetHitTriangleData(triangleDataVec[colI]);

									//hitを呼び出す
									obj1->Hit
									(
										*obj2,
										ShapeType3D::TRIANGLE,
										triangleData.first,
										ShapeType3D::SEGMENT,
										segmentData.first
									);
									obj2->Hit
									(
										*obj1,
										ShapeType3D::SEGMENT,
										segmentData.first,
										ShapeType3D::TRIANGLE,
										triangleData.first
									);

								}
							}
						}

					}
				}
			}

			if (collisionFlags[objJ].triangle
				&& collisionFlags[objI].segment)
			{

				std::unordered_map < std::string, std::vector<TriangleData>>triangleDatas = obj2->GetTriangleDatas();
				std::unordered_map < std::string, std::vector<Segment3DData>>segmentDatas = obj1->GetSegmentDatas();

				// 名前分ループ
				for (const auto& triangleData : triangleDatas)
				{
					for (const auto& segmentData : segmentDatas)
					{

						std::vector<TriangleData>triangleDataVec = triangleData.second;
						size_t triangleDataSize = triangleDataVec.size();
						std::vector<Segment3DData>segmentDataVec = segmentData.second;
						size_t segmentDataSize = segmentDataVec.size();

						for (int colI = 0; colI < triangleDataSize; colI++)
						{
							for (int colJ = 0; colJ < segmentDataSize; colJ++)
							{
								TriangleCalcResult result1;
								Segment3DCalcResult result2;

								if (Collision::TriangleAndSegment3D(triangleDataVec[colI], &result1, segmentDataVec[colJ], &result2))
								{
									obj2->SetTriangleCalcResult(result1);
									obj1->SetSegmentCalcResult(result2);

									obj1->SetHitTriangleData(triangleDataVec[colI]);
									obj2->SetHitSegment3DData(segmentDataVec[colJ]);

									//hitを呼び出す
									obj2->Hit
									(
										*obj1,
										ShapeType3D::TRIANGLE,
										triangleData.first,
										ShapeType3D::SEGMENT,
										segmentData.first
									);
									obj1->Hit
									(
										*obj2,
										ShapeType3D::SEGMENT,
										segmentData.first,
										ShapeType3D::TRIANGLE,
										triangleData.first
									);

								}
							}
						}
					}
				}
			}

#pragma endregion

#pragma region Ray & Sphere
			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].ray)
			{

				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj1->GetSphereDatas();
				std::unordered_map < std::string, std::vector<RayData>>rayDatas = obj2->GetRayDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreSpherePositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreRayPositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& rayData : rayDatas)
					{
						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<RayData>rayDataVec = rayData.second;
						size_t rayDataSize = rayDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < rayDataSize; colJ++)
							{
								SphereCalcResult result1;
								RayCalcResult result2;



								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj1, ShapeType3D::SPHERE, *obj2, ShapeType3D::RAY);

								SphereData sphere1 = sphereDataVec[colI];
								RayData ray = rayDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = sphere1.GetPosition();
								Vector3 prePos1 = prePositions1[sphereData.first][colI];
								Vector3 pos2 = ray.GetPosition();
								Vector3 prePos2 = prePositions2[rayData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));

								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									sphere1.SetPosition(easing1.PreLerp());
									ray.SetPosition(easing2.PreLerp());


									if (Collision::SphereAndRay
									(
										sphere1,
										&result1,
										ray,
										&result2
									))
									{
										obj1->SetSphereCalcResult(result1);
										obj2->SetRayCalcResult(result2);

										obj1->SetHitRayData(ray);
										obj2->SetHitSphereData(sphere1);

										obj1->SetLerpPosition(sphere1.GetPosition());
										obj2->SetLerpPosition(ray.GetPosition());

										obj1->SetLerpMovePosition(easingMovePos1);
										obj2->SetLerpMovePosition(easingMovePos2);

										//hitを呼び出す
										obj1->Hit
										(
											*obj2,
											ShapeType3D::SPHERE,
											sphereData.first,
											ShapeType3D::RAY,
											rayData.first
										);
										obj2->Hit
										(
											*obj1,
											ShapeType3D::RAY,
											rayData.first,
											ShapeType3D::SPHERE,
											sphereData.first
										);
										break;
									}
								}
							}

						}
					}


				}
			}


			if (collisionFlags[objJ].sphere
				&& collisionFlags[objI].ray)
			{
				std::unordered_map < std::string, std::vector<SphereData>>sphereDatas = obj2->GetSphereDatas();
				std::unordered_map < std::string, std::vector<RayData>>rayDatas = obj1->GetRayDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreRayPositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreSpherePositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& rayData : rayDatas)
					{

						std::vector<SphereData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<RayData>rayDataVec = rayData.second;
						size_t boxDataSize = rayDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < boxDataSize; colJ++)
							{
								SphereCalcResult result1;
								RayCalcResult result2;

								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj2, ShapeType3D::SPHERE, *obj1, ShapeType3D::RAY);

								SphereData sphere = sphereDataVec[colI];
								RayData ray = rayDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = ray.GetPosition();
								Vector3 prePos1 = prePositions1[rayData.first][colI];
								Vector3 pos2 = sphere.GetPosition();
								Vector3 prePos2 = prePositions2[sphereData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));
								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									sphere.SetPosition(easing2.PreLerp());
									ray.SetPosition(easing1.PreLerp());

									if (Collision::SphereAndRay
									(
										sphere,
										&result1,
										ray,
										&result2
									))
									{
										obj2->SetSphereCalcResult(result1);
										obj1->SetRayCalcResult(result2);

										obj2->SetHitSphereData(sphere);
										obj1->SetHitRayData(ray);

										obj2->SetLerpPosition(sphere.GetPosition());
										obj1->SetLerpPosition(ray.GetPosition());

										obj2->SetLerpMovePosition(easingMovePos2);
										obj1->SetLerpMovePosition(easingMovePos1);
										//hitを呼び出す
										obj2->Hit
										(
											*obj1,
											ShapeType3D::SPHERE,
											sphereData.first,
											ShapeType3D::RAY,
											rayData.first
										);
										obj1->Hit
										(
											*obj2,
											ShapeType3D::RAY,
											rayData.first,
											ShapeType3D::SPHERE,
											sphereData.first
										);
										break;
									}
								}
							}
						}
					}
				}

			}

#pragma endregion

#pragma region Ray & Box
			if (collisionFlags[objI].box
				&& collisionFlags[objJ].ray)
			{

				std::unordered_map < std::string, std::vector<BoxData>>boxDatas = obj1->GetBoxDatas();
				std::unordered_map < std::string, std::vector<RayData>>rayDatas = obj2->GetRayDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreBoxPositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreRayPositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : boxDatas)
				{
					for (const auto& rayData : rayDatas)
					{
						std::vector<BoxData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<RayData>rayDataVec = rayData.second;
						size_t rayDataSize = rayDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < rayDataSize; colJ++)
							{
								BoxCalcResult result1;
								RayCalcResult result2;



								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj1, ShapeType3D::BOX, *obj2, ShapeType3D::RAY);

								BoxData box = sphereDataVec[colI];
								RayData ray = rayDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = box.GetPosition();
								Vector3 prePos1 = prePositions1[sphereData.first][colI];
								Vector3 pos2 = ray.GetPosition();
								Vector3 prePos2 = prePositions2[rayData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));

								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									box.SetPosition(easing1.PreLerp());
									ray.SetPosition(easing2.PreLerp());


									if (Collision::BoxAndRay
									(
										box,
										ray,
										&result2
									))
									{
										obj1->SetBoxCalcResult(result1);
										obj2->SetRayCalcResult(result2);

										obj1->SetHitRayData(ray);
										obj2->SetHitBoxData(box);

										obj1->SetLerpPosition(box.GetPosition());
										obj2->SetLerpPosition(ray.GetPosition());

										obj1->SetLerpMovePosition(easingMovePos1);
										obj2->SetLerpMovePosition(easingMovePos2);

										//hitを呼び出す
										obj1->Hit
										(
											*obj2,
											ShapeType3D::BOX,
											sphereData.first,
											ShapeType3D::RAY,
											rayData.first
										);
										obj2->Hit
										(
											*obj1,
											ShapeType3D::RAY,
											rayData.first,
											ShapeType3D::BOX,
											sphereData.first
										);
										break;
									}
								}
							}

						}
					}


				}
			}


			if (collisionFlags[objJ].box
				&& collisionFlags[objI].ray)
			{
				std::unordered_map < std::string, std::vector<BoxData>>boxDatas = obj2->GetBoxDatas();
				std::unordered_map < std::string, std::vector<RayData>>rayDatas = obj1->GetRayDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreRayPositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreBoxPositions(prePositions2);


				// 名前分ループ
				for (const auto& sphereData : boxDatas)
				{
					for (const auto& rayData : rayDatas)
					{

						std::vector<BoxData>sphereDataVec = sphereData.second;
						size_t sphereDataSize = sphereDataVec.size();
						std::vector<RayData>rayDataVec = rayData.second;
						size_t boxDataSize = rayDataVec.size();

						for (int colI = 0; colI < sphereDataSize; colI++)
						{
							for (int colJ = 0; colJ < boxDataSize; colJ++)
							{
								BoxCalcResult result1;
								RayCalcResult result2;

								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj2, ShapeType3D::BOX, *obj1, ShapeType3D::RAY);

								BoxData box = sphereDataVec[colI];
								RayData ray = rayDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = ray.GetPosition();
								Vector3 prePos1 = prePositions1[rayData.first][colI];
								Vector3 pos2 = box.GetPosition();
								Vector3 prePos2 = prePositions2[sphereData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));
								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									box.SetPosition(easing2.PreLerp());
									ray.SetPosition(easing1.PreLerp());

									if (Collision::BoxAndRay
									(
										box,
										ray,
										&result2
									))
									{
										obj2->SetBoxCalcResult(result1);
										obj1->SetRayCalcResult(result2);

										obj2->SetHitBoxData(box);
										obj1->SetHitRayData(ray);

										obj2->SetLerpPosition(box.GetPosition());
										obj1->SetLerpPosition(ray.GetPosition());

										obj2->SetLerpMovePosition(easingMovePos2);
										obj1->SetLerpMovePosition(easingMovePos1);
										//hitを呼び出す
										obj2->Hit
										(
											*obj1,
											ShapeType3D::BOX,
											sphereData.first,
											ShapeType3D::RAY,
											rayData.first
										);
										obj1->Hit
										(
											*obj2,
											ShapeType3D::RAY,
											rayData.first,
											ShapeType3D::BOX,
											sphereData.first
										);
										break;
									}
								}
							}
						}
					}
				}

			}

#pragma endregion

#pragma region OBB & Ray
			if (collisionFlags[objI].obb
				&& collisionFlags[objJ].ray)
			{

				std::unordered_map < std::string, std::vector<OBBData>>obbDatas = obj1->GetOBBDatas();
				std::unordered_map < std::string, std::vector<RayData>>rayDatas = obj2->GetRayDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreOBBPositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreRayPositions(prePositions2);


				// 名前分ループ
				for (const auto& obbData : obbDatas)
				{
					for (const auto& rayData : rayDatas)
					{
						std::vector<OBBData>obbDataVec = obbData.second;
						size_t obbDataSize = obbDataVec.size();
						std::vector<RayData>rayDataVec = rayData.second;
						size_t rayDataSize = rayDataVec.size();

						for (int colI = 0; colI < obbDataSize; colI++)
						{
							for (int colJ = 0; colJ < rayDataSize; colJ++)
							{
								//OBBCalcResult result1;
								RayCalcResult result2;



								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj1, ShapeType3D::OBB, *obj2, ShapeType3D::RAY);

								OBBData obb1 = obbDataVec[colI];
								RayData ray = rayDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = obb1.GetPosition();
								Vector3 prePos1 = prePositions1[obbData.first][colI];
								Vector3 pos2 = ray.GetPosition();
								Vector3 prePos2 = prePositions2[rayData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));

								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									obb1.SetPosition(easing1.PreLerp());
									ray.SetPosition(easing2.PreLerp());


									if (Collision::OBBAndRay
									(
										obb1,
										//result1,
										ray,
										&result2
									))
									{
										//obj1->SetOBBCalcResult(result1);
										obj2->SetRayCalcResult(result2);

										obj1->SetHitRayData(ray);
										obj2->SetHitOBBData(obb1);

										obj1->SetLerpPosition(obb1.GetPosition());
										obj2->SetLerpPosition(ray.GetPosition());

										obj1->SetLerpMovePosition(easingMovePos1);
										obj2->SetLerpMovePosition(easingMovePos2);

										//hitを呼び出す
										obj1->Hit
										(
											*obj2,
											ShapeType3D::OBB,
											obbData.first,
											ShapeType3D::RAY,
											rayData.first
										);
										obj2->Hit
										(
											*obj1,
											ShapeType3D::RAY,
											rayData.first,
											ShapeType3D::OBB,
											obbData.first
										);
										break;
									}
								}
							}

						}
					}


				}
			}


			if (collisionFlags[objJ].obb
				&& collisionFlags[objI].ray)
			{
				std::unordered_map < std::string, std::vector<OBBData>>obbDatas = obj2->GetOBBDatas();
				std::unordered_map < std::string, std::vector<RayData>>rayDatas = obj1->GetRayDatas();

				std::unordered_map<std::string, std::vector<Vector3>>prePositions1;
				obj1->GetPreRayPositions(prePositions1);
				std::unordered_map<std::string, std::vector<Vector3>>prePositions2;
				obj2->GetPreOBBPositions(prePositions2);


				// 名前分ループ
				for (const auto& obbData : obbDatas)
				{
					for (const auto& rayData : rayDatas)
					{

						std::vector<OBBData>obbDataVec = obbData.second;
						size_t obbDataSize = obbDataVec.size();
						std::vector<RayData>rayDataVec = rayData.second;
						size_t boxDataSize = rayDataVec.size();

						for (int colI = 0; colI < obbDataSize; colI++)
						{
							for (int colJ = 0; colJ < boxDataSize; colJ++)
							{
								//OBBCalcResult result1;
								RayCalcResult result2;

								// 判定を行う回数を取得
								checkNum = getCheckNum(*obj2, ShapeType3D::OBB, *obj1, ShapeType3D::RAY);

								OBBData obb = obbDataVec[colI];
								RayData ray = rayDataVec[colJ];

								// 座標を補完
								Vector3 pos1 = ray.GetPosition();
								Vector3 prePos1 = prePositions1[rayData.first][colI];
								Vector3 pos2 = obb.GetPosition();
								Vector3 prePos2 = prePositions2[obbData.first][colJ];

								if (pos1 == prePos1 && pos2 == prePos2)checkNum = 1;

								Easing<Vector3>easing1(prePos1, pos1, 100.0f / static_cast<float>(checkNum));
								Easing<Vector3>easing2(prePos2, pos2, 100.0f / static_cast<float>(checkNum));
								Vector3 easingMovePos1 = easing1.GetFrameLarpValue();
								Vector3 easingMovePos2 = easing2.GetFrameLarpValue();
								for (int c = 0; c < checkNum; c++)
								{
									obb.SetPosition(easing2.PreLerp());
									ray.SetPosition(easing1.PreLerp());

									if (Collision::OBBAndRay
									(
										obb,
										//result1,
										ray,
										&result2
									))
									{
										//obj2->SetOBBCalcResult(result1);
										obj1->SetRayCalcResult(result2);

										obj2->SetHitOBBData(obb);
										obj1->SetHitRayData(ray);

										obj2->SetLerpPosition(obb.GetPosition());
										obj1->SetLerpPosition(ray.GetPosition());

										obj2->SetLerpMovePosition(easingMovePos2);
										obj1->SetLerpMovePosition(easingMovePos1);
										//hitを呼び出す
										obj2->Hit
										(
											*obj1,
											ShapeType3D::OBB,
											obbData.first,
											ShapeType3D::RAY,
											rayData.first
										);
										obj1->Hit
										(
											*obj2,
											ShapeType3D::RAY,
											rayData.first,
											ShapeType3D::OBB,
											obbData.first
										);
										break;
									}
								}
							}
						}
					}
				}

			}

#pragma endregion


		}
	}

#ifdef _DEBUG
	for (int i = 0; i < objectSize; i++)
	{
		objects[i]->CreateCollisionCheckModel();
		objects[i]->SetCollisionCheckModelData();

	}
#endif // _DEBUG




	objectSize = object2Ds.size();

	std::vector<CollisionDetectionFlag2D>collisionFlag2Ds(objectSize);
	for (int i = 0; i < objectSize; i++)
	{
		collisionFlag2Ds[i] = object2Ds[i]->GetCollisionFlag();
	}

	for (int objI = 0; objI < objectSize; objI++)
	{
		GameObject2D* obj1 = object2Ds[objI].get();
		for (int objJ = 0; objJ < objectSize; objJ++)
		{
			GameObject2D* obj2 = object2Ds[objJ].get();

			////自分と比較、比較済の組み合わせはcontinue
			if (objI >= objJ)continue;

#pragma region Circle&Circle
			if (collisionFlag2Ds[objI].circle
				&& collisionFlag2Ds[objJ].circle)
			{

				std::vector<CircleData>circleData1 = obj1->GetCircleData();
				size_t circleData1Size = circleData1.size();
				std::vector<CircleData>circleData2 = obj2->GetCircleData();
				size_t circleData2Size = circleData2.size();


				for (int colI = 0; colI < circleData1Size; colI++)
				{
					for (int colJ = 0; colJ < circleData2Size; colJ++)
					{
						if (Collision::CircleAndCircle(circleData1[colI], circleData2[colJ]))
						{
							//hitを呼び出す
							obj1->SetHitCircleData(circleData2[colJ]);
							obj2->SetHitCircleData(circleData1[colJ]);

							obj1->Hit
							(
								obj2,
								ShapeType2D::CIRCLE,
								colI,
								ShapeType2D::CIRCLE,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType2D::CIRCLE,
								colJ,
								ShapeType2D::CIRCLE,
								colI
							);
						}
					}
				}

			}
#pragma endregion

#pragma region Rect&Rect
			if (collisionFlag2Ds[objI].rect
				&& collisionFlag2Ds[objJ].rect)
			{

				std::vector<RectData>rectData1 = obj1->GetRectData();
				size_t rectData1Size = rectData1.size();
				std::vector<RectData>rectData2 = obj2->GetRectData();
				size_t rectData2Size = rectData2.size();


				for (int colI = 0; colI < rectData1Size; colI++)
				{
					for (int colJ = 0; colJ < rectData2Size; colJ++)
					{
						if (Collision::RectAndRect(rectData1[colI], rectData2[colJ]))
						{
							//hitを呼び出す
							obj1->SetHitRectData(rectData2[colJ]);
							obj2->SetHitRectData(rectData1[colJ]);

							obj1->Hit
							(
								obj2,
								ShapeType2D::RECT,
								colI,
								ShapeType2D::RECT,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType2D::RECT,
								colJ,
								ShapeType2D::RECT,
								colI
							);
						}
					}
				}

			}
#pragma endregion


		}
	}

#pragma endregion

	EraseObjectCheck();

}

void GameObjectManager::Draw()
{
	for (auto& o : objects)
	{
		o->Draw();

#ifdef _DEBUG

		o->DrawCollisionCheckModel();
#endif // _DEBUG
	}

	for (auto& o : object2Ds)
	{
		o->Draw();
	}
}

void GameObjectManager::EraseObjectCheck()
{

	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		if (objects[i]->GetEraseManager())
		{
			objects.erase(objects.begin() + i);
			i--;
			size--;
		}
	}

	objects.shrink_to_fit();

	size = object2Ds.size();
	for (size_t i = 0; i < size; i++)
	{
		if (object2Ds[i]->GetEraseManager())
		{
			object2Ds.erase(object2Ds.begin() + i);
			i--;
			size--;
		}
	}

	object2Ds.shrink_to_fit();
}

void GameObjectManager::Finalize()
{
	AllEraseObject();
}

#pragma region オブジェクト関数
void GameObjectManager::ReserveObjectArray(const int reserveNum)
{
	objects.reserve(reserveNum);
}

void GameObjectManager::AddObject(const std::shared_ptr<GameObject>& object)
{
	if (object)
	{
		object.get()->FalseEraseManager();
		addObjects.push_back(object);

	}
}

void GameObjectManager::AddObject(const std::shared_ptr<GameObject2D>& object)
{
	if (object)
	{
		object.get()->FalseEraseManager();
		addObject2Ds.push_back(object);
	}
}

void GameObjectManager::SetAddObjectSortState(const ObjectSortType& sort, const bool& orderType)
{
	addObjectSort = sort;
	addObjectSortOrderType = orderType;
}

void GameObjectManager::ObjectSort(const ObjectSortType& sort, const bool& orderType)
{
	switch (sort)
	{
	case OBJECT_SORT_XYZ_SUM:
		std::sort(objects.begin(), objects.end(), [&orderType](const std::shared_ptr<GameObject>& obj1, const std::shared_ptr<GameObject>& obj2)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();
			float posSum1 = pos1.x + pos1.y + pos1.z;
			float posSum2 = pos2.x + pos2.y + pos2.z;

			if (orderType)return posSum1 < posSum2;
			return posSum1 > posSum2;
		});
		break;

	case OBJECT_SORT_X:
		std::sort
		(
			objects.begin(),
			objects.end(),
			[&orderType]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
			)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			if (orderType)return pos1.x < pos2.x;
			return pos1.x > pos2.x;
		});
		break;

	case OBJECT_SORT_Y:
		std::sort
		(
			objects.begin(),
			objects.end(),
			[&orderType]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
			)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			if (orderType)return pos1.y < pos2.y;
			return pos1.y > pos2.y;
		});
		break;

	case OBJECT_SORT_Z:
		std::sort
		(
			objects.begin(),
			objects.end(),
			[&orderType]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
			)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			if (orderType)return pos1.z < pos2.z;
			return pos1.z > pos2.z;
		});
		break;

	case OBJECT_SORT_NEAR_DISTANCE:

		std::sort
		(
			objects.begin(),
			objects.end(),
			[&]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
			)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			float dis1 = LibMath::CalcDistance3D(pos1, nearPos);
			float dis2 = LibMath::CalcDistance3D(pos2, nearPos);

			if (orderType)return dis1 < dis2;
			return dis1 > dis2;
		});
		break;

	case OBJECT_SORT_FAR_DISTANCE:

		std::sort
		(
			objects.begin(),
			objects.end(),
			[&]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
			)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			float dis1 = LibMath::CalcDistance3D(pos1, farPos);
			float dis2 = LibMath::CalcDistance3D(pos2, farPos);

			if (orderType)return dis1 < dis2;
			return dis1 > dis2;
		});
		break;

	case OBJECT_SORT_SORT_NUMBER:
		std::sort
		(
			objects.begin(),
			objects.end(),
			[&]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
			)
		{
			short obj1Num = obj1->GetSortNumber();
			short obj2Num = obj2->GetSortNumber();
			if (orderType)return obj1Num < obj2Num;
			return obj1Num > obj2Num;
		});
		break;
	}
}

#pragma endregion

//void GameObjectManager::SetCollisionFlag3D(const CollisionFlag& type)
//{
//	checkCollision = type;
//}

void GameObjectManager::SetMouseCollisionFlag(const bool flag)
{
	checkMouseCollision = flag;
}



void GameObjectManager::AllEraseObject()
{
	objects.clear();
	object2Ds.clear();
}

void MelLib::GameObjectManager::AllEraseObject2D()
{
	object2Ds.clear();
}

void MelLib::GameObjectManager::AllEraseObject3D()
{
	objects.clear();
}
