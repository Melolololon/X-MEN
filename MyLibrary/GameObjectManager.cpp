#include "GameObjectManager.h"
#include"CollisionType.h"
#include"LibMath.h"
#include"Collision.h"
#include"Physics.h"
#include"Values.h"


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
		obj->Update();

	}

	for (auto& obj : object2Ds)
	{
		obj->Update();

	}

	if (addObjects.size() != 0)
	{
		for (auto& a : addObjects)
		{

			a.get()->Update();
			objects.push_back(a);
		}

		if (addObjectSort != OBJECT_SORT_NONE)
			ObjectSort(addObjectSort, addObjectSortOrderType);

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

#pragma region Sphere & Sphere


			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].sphere)
			{

				std::unordered_map<std::string, std::vector<SphereData>> sphereDatas1 = obj1->GetSphereDatas();
				std::unordered_map<std::string, std::vector<SphereData>> sphereDatas2 = obj2->GetSphereDatas();

				// 名前分ループ
				for (const auto& shapeDatas1 : sphereDatas1)
				{
					for (const auto& shapeDatas2 : sphereDatas1)
					{
						std::vector<SphereData>sphereDataVec1 = sphereDatas1[shapeDatas1.first];
						size_t sphereData1Size = sphereDataVec1.size();
						std::vector<SphereData>sphereDataVec2 = sphereDatas2[shapeDatas1.first];
						size_t sphereData2Size = sphereDataVec2.size();


						for (int colI = 0; colI < sphereData1Size; colI++)
						{
							for (int colJ = 0; colJ < sphereData2Size; colJ++)
							{
								if (Collision::SphereAndSphere(sphereDataVec1[colI], sphereDataVec2[colJ]))
								{
									//hitを呼び出す
									obj1->SetHitSphereData(sphereDataVec2[colJ]);
									obj2->SetHitSphereData(sphereDataVec1[colJ]);

									obj1->Hit
									(
										*obj2,
										ShapeType3D::SPHERE,
										shapeDatas1.first,
										ShapeType3D::SPHERE,
										shapeDatas2.first
									);
									obj2->Hit
									(
										*obj1,
										ShapeType3D::SPHERE,
										shapeDatas2.first,
										ShapeType3D::SPHERE,
										shapeDatas1.first
									);
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
				std::unordered_map < std::string, std::vector<BoxData>>boxData = obj2->GetBoxDatas();

				// 名前分ループ
				for (const auto& sphereData : sphereDatas)
				{
					for (const auto& boxData : boxData)
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

								if (Collision::SphereAndBox
								(
									sphereDataVec[colI],
									&result1,
									boxDataVec[colJ],
									&result2
								))
								{
									obj1->SetSphereCalcResult(result1);
									obj2->SetBoxCalcResult(result2);

									obj1->SetHitBoxData(boxDataVec[colJ]);
									obj2->SetHitSphereData(sphereDataVec[colI]);

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

								if (Collision::SphereAndBox
								(
									sphereDataVec[colI],
									&result1,
									boxDataVec[colJ],
									&result2
								))
								{
									obj2->SetSphereCalcResult(result1);
									obj1->SetBoxCalcResult(result2);

									obj1->SetHitSphereData(sphereDataVec[colI]);
									obj2->SetHitBoxData(boxDataVec[colJ]);

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
