#include "yaMapCollider.h"
#include "yaObject.h"
#include "yaWallScript.h"
#include "yaCollider2D.h"
namespace ya
{
	MapCollider::MapCollider()
		: mGroundCount(0)
		, mWallCount(0)
	{
	}
	MapCollider::~MapCollider()
	{
	}
	void MapCollider::Initialize()
	{
		//성복
		SetName(L"mapColliders");
		mWalkerName = L"KSB";
		AddWallCollider(Vector3(96.5, -20, 20), Vector3(0, -25, 0), Vector3(8, 9, 0.1));
		AddWallCollider(Vector3(104.000, -20, 21.700), Vector3(0, 0.000, 0), Vector3(8, 9, 0.1));
		AddWallCollider(Vector3(107.000, -20, 21.000), Vector3(0, 40.000, 0), Vector3(2.000, 9, 0.1));
		AddWallCollider(Vector3(112.000, -20, 20.500), Vector3(0, -5.000, 0), Vector3(8.000, 11.000, 0.1));
		AddWallCollider(Vector3(118.000, -22.000, 19.800), Vector3(0, 40.000, 0), Vector3(8.000, 5.000, 0.1));
		AddWallCollider(Vector3(124.000, -22.000, 21.000), Vector3(0, -50.000, 0), Vector3(10.000, 5.000, 0.1));
		AddWallCollider(Vector3(110.000, -19.000, -13.500), Vector3(0, -5, 0), Vector3(15.000, 13.000, 0.1));
		AddWallCollider(Vector3(103.500, -19.000, -19.000), Vector3(0, 85.000, 6.000), Vector3(11.000, 13.000, 0.1));
		AddWallCollider(Vector3(99.500, -22.000, -17.000), Vector3(0, 10.000, 0), Vector3(8.000, 7, 0.1));
		AddWallCollider(Vector3(97.500, -22.000, -16.000), Vector3(0, -50.000, 0), Vector3(3.500, 5.000, 0.1));
		AddWallCollider(Vector3(100.000, -22.000, -16.000), Vector3(0, 50.000, 0), Vector3(3.800, 5.000, 0.1));
		AddWallCollider(Vector3(114.500, -22.000, -11.000), Vector3(0, 90.000, 0), Vector3(6.000, 5.000, 0.1));
		AddWallCollider(Vector3(117.000, -21.000, -4.000), Vector3(0, 115.000, 0), Vector3(9.000, 10.000, 0.1));
		AddWallCollider(Vector3(123.000, -20.000, 5.000), Vector3(0, -25.000, 0), Vector3(12.000, 7, 0.1));
		AddWallCollider(Vector3(118.000, -20.000, 0.000), Vector3(0, 70.000, 0), Vector3(4.000, 8.000, 0.1));
		AddWallCollider(Vector3(130.0, -22.000, 8.000), Vector3(0, -30.000, 0), Vector3(5.000, 5.000, 0.1));
		AddWallCollider(Vector3(131.500, -22.000, 15.500), Vector3(0, -50.000, 0), Vector3(5.000, 5.000, 0.1));
		AddWallCollider(Vector3(127.000, -22.000, 18.000), Vector3(0, -50.000, 0), Vector3(4.000, 5.000, 0.1));
		AddWallCollider(Vector3(131.000, -22.000, 12.000), Vector3(0, 40.000, 0), Vector3(4.000, 5.000, 0.1));
		AddWallCollider(Vector3(124.000, -22.000, 18.000), Vector3(0, 40.000, 0), Vector3(5.000, 5.000, 0.1));
		AddWallCollider(Vector3(127.500, -17.000, 14.800), Vector3(0, 40.000, 0), Vector3(17.500, 4.000, 0.1));
		AddWallCollider(Vector3(136.500, -17.000, 11.200), Vector3(0, 130.000, 0), Vector3(5.600, 4.000, 0.1));
		AddWallCollider(Vector3(131.500, -17.000, 19.000), Vector3(0, 40.000, 0), Vector3(17.500, 4.000, 0.1));
		AddWallCollider(Vector3(123.000, -17.000, 22.000), Vector3(0, 130.000, 0), Vector3(5.600, 4.000, 0.1));
		AddWallCollider(Vector3(119.000, -15.000, 24.500), Vector3(0, 120.000, 0), Vector3(10.000, 8.000, 0.1));

		//마굿간
		AddWallCollider(Vector3(99.300, -21.900, 11.000), Vector3(0, 95.000, 0), Vector3(18.000, 5.000, 0.1));
		AddWallCollider(Vector3(104.500, -21.900, 10.500), Vector3(0, 95.000, 0), Vector3(18.000, 5.000, 0.1));
		AddWallCollider(Vector3(101.000, -21.900, 1.900), Vector3(0, 5.000, 0), Vector3(5.000, 5.000, 0.1));
		AddWallCollider(Vector3(103.000, -21.900, 20.000), Vector3(0, 5.000, 0), Vector3(5.000, 5.000, 0.1));


		AddGroundCollider(Vector3(116.000, -24.00, 2), Vector3(0, -5, 0), Vector3(40.000, 1.000, 40));
		AddGroundCollider(Vector3(111.000, -13.000, -19.000), Vector3(0, -5, 0), Vector3(13.000, 1.000, 10.000));
		AddGroundCollider(Vector3(124.000, -16.500, -2.000), Vector3(10.000, -45.000, -5.000), Vector3(20.000, 1.000, 6.000));
		AddGroundCollider(Vector3(124.500, -17.00, 2.800), Vector3(0, -25.000, 0), Vector3(11.000, 1.000, 6.000));
		AddGroundCollider(Vector3(121.000, -19.700, 23.500), Vector3(0, 42.000, 0), Vector3(9.000, 1.000, 9.000));
		AddGroundCollider(Vector3(100.500, -18.500, 11.000), Vector3(2.000, 6.000, 30.000), Vector3(3.000, 0.500, 21.000));
		AddGroundCollider(Vector3(103.300, -18.500, 11.000), Vector3(-3.500, 6.000, -30.000), Vector3(3.000, 0.500, 21.000));
		AddGroundCollider(Vector3(105.500, -20.000, 11.000), Vector3(-0.000, 6.000, -10.000), Vector3(4.000, 0.500, 21.000));
		AddGroundCollider(Vector3(128.000, -13.800, 15.000), Vector3(-35.000, 39.000, 0), Vector3(18.500, 1.000, 6.000));
		AddGroundCollider(Vector3(131.000, -13.800, 18.500), Vector3(35.000, 39.000, 0), Vector3(18.500, 1.000, 6.000));
		AddGroundCollider(Vector3(137.000, -19.800, 10.500), Vector3(0, 42.000, 0), Vector3(4.500, 1.000, 8.500));


		///텐젠존

		AddWallCollider(Vector3(-4.000, -6.000, -20.000), Vector3(0, 85.000, 0), Vector3(32.000, 7.000, 0.1));
		AddWallCollider(Vector3(-2.000, -2.000, -35.000), Vector3(0, -6.000, 0), Vector3(24.000, 10.000, 0.1));
		AddWallCollider(Vector3(10.500, -2.000, -40.000), Vector3(0, 85.000, 0), Vector3(12.000, 10.000, 0.1));
		AddWallCollider(Vector3(20.0, -2.000, -39.000), Vector3(0, 85.000, 0), Vector3(12.000, 10.000, 0.1));
		AddWallCollider(Vector3(-13.700, -2.000, -42.000), Vector3(0, 85.000, 0), Vector3(12.000, 10.000, 0.1));
		AddWallCollider(Vector3(-2.000, -2.000, -46.500), Vector3(0, -4.500, 0), Vector3(24.000, 10.000, 0.1));
		AddWallCollider(Vector3(27.487, -5.178, -3.107), Vector3(0, -5.000, 0), Vector3(11.428, 7.479, 0.1));

		AddWallCollider(Vector3(-5.108, -5.969, -4.7427), Vector3(0, -1.255, 0), Vector3(5.0446, 12.735, 0.1));
		AddWallCollider(Vector3(-0.077, -6.704, -4.7427), Vector3(0, -1.255, 0), Vector3(5.0446, 3.5003, 0.1));
		AddWallCollider(Vector3(-1.125, -4.176, -4.7427), Vector3(0, -1.255, 0), Vector3(2.8605, 1.7040, 0.1));
		AddWallCollider(Vector3(18.388, -5.750, -3.0268), Vector3(0, -1.255, 0), Vector3(2.8605, 6.2458, 0.1));
		AddWallCollider(Vector3(15.650, -6.265, -3.0268), Vector3(0, -1.255, 0), Vector3(2.8605, 4.1471, 0.1));
		AddWallCollider(Vector3(13.020, -7.333, -3.0268), Vector3(0, -1.255, 0), Vector3(2.8657, 2.2584, 0.1));
		AddWallCollider(Vector3(21.258, -6.762, -2.9982), Vector3(0, 13.485, 0), Vector3(2.8657, 10.882, 0.1));
		AddWallCollider(Vector3(31.416, -6.805, -3.9501), Vector3(0, 79.479, 0), Vector3(2.2697, 10.882, 0.1));
		AddWallCollider(Vector3(36.176, -6.806, -5.1098), Vector3(0, -2.861, 0), Vector3(9.6052, 10.882, 0.1));
		AddWallCollider(Vector3(38.381, -6.632, -7.6456), Vector3(0, 83.313, 0), Vector3(7.0116, 10.873, 0.1));
		AddWallCollider(Vector3(37.207, -5.061, -10.261), Vector3(0, -10.552, 0), Vector3(3.7544, 7.6859, 0.1));
		AddWallCollider(Vector3(35.484, -5.612, -10.813), Vector3(0, -97.179, 0), Vector3(0.9136, 7.6859, 0.1));
		AddWallCollider(Vector3(37.252, -6.192, -10.809), Vector3(0, -177.16, 0), Vector3(3.2001, 7.6859, 0.1));
		AddWallCollider(Vector3(37.916, -6.192, -15.821), Vector3(0, -187.84, 0), Vector3(3.2001, 7.6859, 0.1));
		AddWallCollider(Vector3(36.114, -5.925, -16.376), Vector3(0, -275.12, 0), Vector3(0.9184, 7.6859, 0.1));
		AddWallCollider(Vector3(37.782, -5.386, -16.441), Vector3(0, -366.75, 0), Vector3(3.6520, 7.6859, 0.1));
		AddWallCollider(Vector3(39.761, -5.471, -19.929), Vector3(0, -273.59, 0), Vector3(7.0512, 7.6859, 0.1));
		AddWallCollider(Vector3(38.032, -5.661, -22.290), Vector3(0, -185.65, 0), Vector3(8.7183, 7.6859, 0.1));
		AddWallCollider(Vector3(33.737, -5.729, -28.007), Vector3(0, -96.480, 0), Vector3(10.312, 8.1832, 0.1));
		AddWallCollider(Vector3(27.140, -3.502, -32.740), Vector3(0, -4.6219, 0), Vector3(14.824, 12.923, 0.1));
		AddWallCollider(Vector3(17.164, -3.510, -38.849), Vector3(0, -22.622, 0), Vector3(3.8790, 8.6987, 0.1));
		AddWallCollider(Vector3(20.208, -4.716, -37.921), Vector3(0, -9.0162, 0), Vector3(2.3788, 8.6987, 0.1));

		AddWallCollider(Vector3(20.024, -2.137, 7.9242), Vector3(0, 87.2665, 0), Vector3(3.5341, 1.1746, 0.1));
		AddWallCollider(Vector3(19.514, -2.404, 0.7408), Vector3(0, 171.366, 0), Vector3(3.0213, 1.7466, 0.1));
		AddWallCollider(Vector3(19.270, -2.404, 6.4628), Vector3(0, 173.523, 0), Vector3(3.0213, 1.7466, 0.1));
		AddWallCollider(Vector3(-5.259, -1.585, -3.549), Vector3(0, 174.711, 0), Vector3(5.1600, 4.3748, 0.1));
		AddWallCollider(Vector3(-6.987, -1.570, -10.42), Vector3(0, 85.2750, 0), Vector3(13.589, 4.3748, 0.1));
		AddWallCollider(Vector3(-6.001, -1.570, -16.83), Vector3(0, -3.4880, 0), Vector3(0.9013, 3.8043, 0.1));
		AddWallCollider(Vector3(-5.200, -1.570, -19.07), Vector3(0, -3.4880, 0), Vector3(2.200, 3.8043, 0.1));
		AddWallCollider(Vector3(-6.008, -1.570, -21.60), Vector3(0, 85.4509, 0), Vector3(5.3673, 3.8043, 0.1));
		AddWallCollider(Vector3(-4.000, -1.570, -21.60), Vector3(0, 85.4509, 0), Vector3(5.3673, 3.8043, 0.1));
		AddWallCollider(Vector3(-4.800, -1.570, -24.22), Vector3(0, -10.728, 0), Vector3(2.500, 3.8043, 0.1));
		AddWallCollider(Vector3(-17.09, -1.570, 1.2543), Vector3(0, -3.7733, 0), Vector3(13.472, 33.406, 0.1));
		AddWallCollider(Vector3(-11.40, -1.570, 8.8553), Vector3(0, 83.3073, 0), Vector3(13.764, 33.406, 0.1));
		AddWallCollider(Vector3(25.412, -1.570, 3.8106), Vector3(0, 87.8189, 0), Vector3(14.962, 6.4707, 0.1));

		AddGroundCollider(Vector3(25.412, -1.570, 3.8106), Vector3(0, 0, 0), Vector3(14.962, 6.4707, 2));  //
		AddGroundCollider(Vector3(-12.94, -7.8619, -3.965), Vector3(0, -4.857, 0), Vector3(13.654, 10, 20.000));
		AddGroundCollider(Vector3(-7.844, -7.8619, -25.73), Vector3(0, -4.857, 0), Vector3(8.7088, 10, 24.145));		

		AddGroundCollider(Vector3(7.011, -11.000, -3.500), Vector3(-18.000, -4.8569, 0.0000), Vector3(10.000, 10.000, 20.000)); // 14
		AddGroundCollider(Vector3(0.000, -9.000, 0.5), Vector3(0, -4.000, -25.000), Vector3(7.400, 10.00, 8.000));
		AddGroundCollider(Vector3(14.184, -9.300, 1.500), Vector3(-5.000, 0.000, 20.000), Vector3(7.000, 10.000, 8.000));

		AddGroundCollider(Vector3( -3.91,-7.8618, 1.7890), Vector3(0.0000, -4.8569, 0.0000), Vector3(5.2156, 10.000, 10.446)); // 17
		AddGroundCollider(Vector3( 9.097,-7.9458, 6.500), Vector3(0, -4.856,  0.0000), Vector3(21.865, 10.00 , 4.000));
		//AddGroundCollider(Vector3( -0.41,-8.8318, 1.0564), Vector3(1.9803, 16.7396, 0.0000), Vector3(2.8238, 10.000, 7.8979));

		AddGroundCollider(Vector3( 23.041,-8.000,  2.411), Vector3(0, -1.994, 0), Vector3(15.000, 10.00, 10.000)); // 20
		//AddGroundCollider(Vector3(30.961, -12.318, -27.50), Vector3(6.142, -4.812, -1.26), Vector3(6.244, 10.00, 10.54));
		AddGroundCollider(Vector3(10.000, -10.000, -20.000), Vector3( 0, -0, -0), Vector3(60.000, 4, 60.000));

		//AddGroundCollider(Vector3(0.6593, -18.362, -29.20), Vector3( 3.6196, 16.739, -3.2266), Vector3(16.527,19.95, 15.76));
		//AddGroundCollider(Vector3(8.0972, -17.122, -3.019), Vector3( -15.39, -3.516,  0.6036), Vector3(6.2035,19.95, 13.54)); 쓰렉
		AddGroundCollider(Vector3(2.500, -10.700, -5.665), Vector3(-16.000, 5.000, 30.000), Vector3(4.000, 7.000, 8.302));
		AddGroundCollider(Vector3(14.000, -9.800, -4.172), Vector3(-20.000, -12.000, -25.000), Vector3(6.203, 5.000, 8.302));
		AddGroundCollider(Vector3(14.000, -9.800, -4.172), Vector3(-20.000, -12.000, -25.000), Vector3(6.203, 5.000, 8.302));
		//AddGroundCollider(Vector3(27.926, -18.105, -27.11), Vector3( 5.3689, -6.045,  5.8809), Vector3(12.215,19.95, 11.89));
		//은혜
		mWalkerName = L"JU";
		// 14xxxx
		AddGroundCollider(Vector3(190.0, -30.0, 88.0), Vector3(-30.0, -20.0, 5.0), Vector3(13.0, 1.0, 10.0));	// 0
		AddGroundCollider(Vector3(195.0, -30.0, 86.0), Vector3(-20.0, 32.0, 0.0), Vector3(8.0, 1.0, 13.0));		// 1
		AddGroundCollider(Vector3(186.0, -34.0, 80.0), Vector3(-26.0, 30.0, 0.0), Vector3(24.0, 1.0, 10.0));	// 2
		AddGroundCollider(Vector3(180.0, -36.701, 77.0), Vector3(-20.0, 45.0, -8.0), Vector3(20.0, 1.0, 10.0));	// 3
		AddGroundCollider(Vector3(173.0, -37.201, 66.0), Vector3(0.0, 50.0, 0.0), Vector3(12.0, 1.0, 24.0));	// 4
		AddGroundCollider(Vector3(153.0, -37.201, 55.0), Vector3(0.0, 40.0, 0.0), Vector3(12.0, 1.0, 24.0));	// 5
		AddGroundCollider(Vector3(156.0, -32.0, 65.0), Vector3(13.0, 54.0, 14.0), Vector3(8.0, 1.0, 16.0));		// 6
		AddGroundCollider(Vector3(144.0, -26.50, 59.0), Vector3(4.0, 36.0, 10.0), Vector3(9.0, 1.0, 9.0));		// 7

		// 2xxxxx
		AddGroundCollider(Vector3(153.50, -29.50, 41.50), Vector3(0.0, 39.0, 0.0), Vector3(9.0, 1.0, 7.0));		// 8
		AddGroundCollider(Vector3(138.40, -31.10, 53.0), Vector3(0.0, 41.0, 0.0), Vector3(9.0, 1.0, 10.0));		// 9
		AddGroundCollider(Vector3(145.0, -35.3, 47.0), Vector3(27.0, 40.0, -0.0), Vector3(10.0, 1.0, 9.0));		// 10
		AddGroundCollider(Vector3(138.0, -33.50, 38.0), Vector3(0.0, 39.50, 0.0), Vector3(10.0, 1.0, 15.0));	
		
		AddGroundCollider(Vector3(152.2, -24.40, 33.3), Vector3(0.0, 41.0, 0.0), Vector3(16.0, 1.0, 8.50));
		AddGroundCollider(Vector3(146.60, -29.10, 21.0), Vector3(0.0, 41.0, 0.0), Vector3(8.0, 1.0, 18.0));

		AddGroundCollider(Vector3(143.4, -31.40, 30.0), Vector3(24.0, 36.0, 33.0), Vector3(10.40, 1.0, 7.0));
		AddGroundCollider(Vector3(145.10, -27.3, 20.50), Vector3(28.0, 39.50, 2.0), Vector3(5.60, 1.0, 10.0));
		AddGroundCollider(Vector3(131.0, -24.8, 26.50), Vector3(0.0, 40.0, 0.0), Vector3(24.0, 1.0, 13.0));
		AddGroundCollider(Vector3(140.0, -24.8, 15.0), Vector3(0.0, 40.0, 0.0), Vector3(6.0, 1.0, 7.0));
		AddGroundCollider(Vector3(127.50, -24.8, 41.0), Vector3(0.0, 40.0, 0.0), Vector3(10.0, 1.0, 25.0));

		AddGroundCollider(Vector3(141.7, -20.9, 21.8), Vector3(0.0, 40.0, 0.0), Vector3(1.5, 0.2, 7.0));
		
		// 14xxxx
		AddWallCollider(Vector3(169.0, -32.37401, 71.0), Vector3(0.0, -50.0, 0.0), Vector3(12.0, 13.0, 1.0));
		AddWallCollider(Vector3(159.0, -34.50, 62.50), Vector3(0.0, -33.0, 0.0), Vector3(17.0, 6.0, 1.0));
		AddWallCollider(Vector3(148.0, -31.799999, 56.0), Vector3(0.0, -51.0, 0.0), Vector3(8.0, 12.0, 1.0));

		// 2xxxxx
		AddWallCollider(Vector3(142.0, -34.0, 51.0), Vector3(0.0, -49.0, 0.0), Vector3(9.0, 7.0, 1.0));
		AddWallCollider(Vector3(150.50, -33.0, 45.0), Vector3(1.0, -50.0, -8.0), Vector3(6.0, 7.0, 1.0));
		AddWallCollider(Vector3(156.0, -34.0, 44.50), Vector3(-8.0, 39.0, 0.0), Vector3(10.0, 10.0, 1.0));
		AddWallCollider(Vector3(157.0, -33.0, 39.0), Vector3(4.0, -50.0, 0.0), Vector3(6.0, 7.0, 1.0));

		AddWallCollider(Vector3(146.0, -30.0, 38.50), Vector3(-10.0, -49.0, 0.0), Vector3(9.0, 11.0, 1.0));
		AddWallCollider(Vector3(156.0, -30.0, 37.0), Vector3(-13.0, 40.0, 0.0), Vector3(16.0, 12.0, 1.0));
		AddWallCollider(Vector3(135.0, -30.50, 42.0), Vector3(6.0, -49.0, 0.0), Vector3(15.0, 12.0, 1.0));
		AddWallCollider(Vector3(137.303, -29.0, 30.50), Vector3(-8.0, 40.50, 0.0), Vector3(18.0, 10.0, 1.0));
		AddWallCollider(Vector3(142.0, -24.0, 22.0), Vector3(0.0, -50.0, 0.0), Vector3(7.0, 6.0, 0.80));


		//상수
		mWalkerName = L"KSS";
		AddWallCollider(Vector3(97.50, -22.0, -14.4), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 7.0, 5.0));
		AddWallCollider(Vector3(96.40, -22.0, -4.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 7.0, 5.0));
		AddWallCollider(Vector3(95.0, -20.0, -6.30), Vector3(0.0, -5.0, 0.0), Vector3(3.80, 10.0, 0.30));
		AddWallCollider(Vector3(96.0, -20.0, -12.6), Vector3(0.0, -5.0, 0.0), Vector3(3.80, 10.0, 0.30));
		AddWallCollider(Vector3(96.40, -22.0, -4.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 7.0, 5.0));
		AddWallCollider(Vector3(96.40, -22.0, -4.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 7.0, 5.0));
		AddWallCollider(Vector3(96.0, -22.0, 1.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 4.0, 6.0));
		AddWallCollider(Vector3(96.0, -22.0, 1.0), Vector3(0.0, -5.0, 0.0), Vector3(0.50, 4.0, 6.0)); //
		AddWallCollider(Vector3(91.0, -22.90, 8.0), Vector3(10.0, -7.0, 0.0), Vector3(8.0, 10.0, 1));
		AddWallCollider(Vector3(79.0, -22.90, 3.30), Vector3(7.0, -7.0, 0.0), Vector3(8.0, 10.0, 1));
		AddWallCollider(Vector3(82.40, -23.40, 9.0), Vector3(0.0, -7.0, 10.0), Vector3(1, 10.0, 11.0));
		AddWallCollider(Vector3(87.0, -23.0, 9.0), Vector3(0.0, -7.0, -4.0), Vector3(1.0, 10.0, 4.0));
		AddWallCollider(Vector3(94.70, -22.0, 14.0), Vector3(0.0, -5.0, 10.0), Vector3(1, 8.0, 12.0));
		AddWallCollider(Vector3(78.40, -16.50, 4.20), Vector3(0.0, -5.0, 0.0), Vector3(8.0, 3.0, 0.70));
		AddWallCollider(Vector3(83.0, -17.0, 18.0), Vector3(0.0, -5.0, 0.0), Vector3(24.0, 4.0, 1));
		AddWallCollider(Vector3(68.0, -16.0, 1.0), Vector3(0.0, -23.0, 0.0), Vector3(17.0, 16.0, 1));
		AddWallCollider(Vector3(74.0, -15.0, 9.0), Vector3(0.0, -14.0, 0.0), Vector3(1, 16.0, 11.0));
		AddWallCollider(Vector3(55.50, -18.0, -19.0), Vector3(0.0, -5.0, 10.0), Vector3(1, 14.0, 9.0));
		AddWallCollider(Vector3(53.0, -10.0, -22.0), Vector3(-10.0, -5.0, 0.0), Vector3(17.0, 10.0, 1));
		AddWallCollider(Vector3(43.0, -6.40, -24.0), Vector3(-10.0, -5.0, 0.0), Vector3(7.0, 10.0, 1));
		AddWallCollider(Vector3(41.50, -10.0, -13.50), Vector3(0.0, -5.0, 0.0), Vector3(1, 1, 19.0));
		AddWallCollider(Vector3(39.0, -6.60, -3.70), Vector3(10.0, -5.0, 0.0), Vector3(3.0, 10.0, 1));
		AddWallCollider(Vector3(40.30, -6.60, -2.0), Vector3(0.0, -5.0, 10.0), Vector3(1, 10.0, 3.0));

		AddGroundCollider(Vector3(89.000, -23.000, -5.000), Vector3(-6.000, -4.000, 0), Vector3(15.000, 1.000, 25.000));
		AddGroundCollider(Vector3(77.000, -20.000, -5.000), Vector3(1.000, 0, -14.000), Vector3(20.000, 1.000, 18.000));
		AddGroundCollider(Vector3(43.500, -10.400, -13.000), Vector3(1.000, -5.000, -22.000), Vector3(4.000, 1.000, 9.0));
		AddGroundCollider(Vector3(34.600, -9.500, -14.000), Vector3(0, -5.000, 0), Vector3(15.000, 1.000, 30.000));

		AddGroundCollider(Vector3(97.90, -15.30, -9.10), Vector3(3.80, -5.0, -34.0), Vector3(2.20, 0.30, 10.60));
		AddGroundCollider(Vector3(96.20, -15.30, -9.10), Vector3(-3.0, -5.0, 30.0), Vector3(1.80, 0.30, 10.60));
		AddGroundCollider(Vector3(79.40, -15.0, 4.0), Vector3(0, -6.0, 0), Vector3(6.30, 0.30, 1.40));

		AddGroundCollider(Vector3(84.0, -20.30, 6.0), Vector3(-26.0, -2.0, 2.0), Vector3(6.0, -0.50, 10.0));
		AddGroundCollider(Vector3(90.60, -18.20, 13.20), Vector3(0, -6.0, 0), Vector3(8.0, 1.0, 10.0));
		AddGroundCollider(Vector3(84.0, -18.20, 15.0), Vector3(0, -6.0, 0), Vector3(10.0, 1, 10.0));
		AddGroundCollider(Vector3(77.70, -18.20, 11.0), Vector3(0, -6.0, 0), Vector3(10, 1, 10));
		AddGroundCollider(Vector3(85.0, -15.30, 24.0), Vector3(0, -5.0, 2.0), Vector3(22.0, 1.0, 10.0));
		AddGroundCollider(Vector3(72.0, -15.0, 24.0), Vector3(0, -30.0, 0), Vector3(8.0, 1.0, 10.0));

		AddGroundCollider(Vector3(62.0, -15.0, -9.50), Vector3(0, -4.0, -22.0), Vector3(19.0, 1.0, 8.50));
		AddGroundCollider(Vector3(45.5, -11.0, -12.50), Vector3(0, -5.0, 0), Vector3(18.000, 1.0, 24.0));
		AddGroundCollider(Vector3(49.0, -11.0, 4.50), Vector3(0, -6.0, 0), Vector3(15.0, 1.0, 15.0));
		AddGroundCollider(Vector3(58.0, -8.50, 6.0), Vector3(0, -20.0, 15.0), Vector3(18.0, 1.0, 10.0));
		AddGroundCollider(Vector3(68.0, -6.50, 8.0), Vector3(-4.0, -20.0, 10.0), Vector3(10.0, 1.0, 10.0));
		
	}
	void MapCollider::AddWallCollider(Vector3 transform, Vector3 rotation, Vector3 size)
	{
		GameObject* obj = object::Instantiate<GameObject>(eLayerType::Wall, GetScene());
		std::wstring num = std::to_wstring(mWallCount++);
		obj->SetName(mWalkerName + L"_WallCollider_" + num);
		obj->AddComponent<Collider2D>()->SetType(eColliderType::Box);
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(transform);
		tr->SetRotation(rotation);
		tr->SetScale(size);
		obj->AddComponent<WallScript>();
	}
	void MapCollider::AddGroundCollider(Vector3 transform, Vector3 rotation, Vector3 size)
	{
		GameObject* obj = object::Instantiate<GameObject>(eLayerType::Ground, GetScene());
		std::wstring num = std::to_wstring(mGroundCount++);
		obj->SetName(mWalkerName + L"_GroundCollider_" + num);
		obj->AddComponent<Collider2D>()->SetType(eColliderType::Box);
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(transform);
		tr->SetRotation(rotation);
		tr->SetScale(size);
	}
}