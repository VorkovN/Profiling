#include <iostream>
#include <thread>
#include <cmath>
#include <vector>
#include <fstream>

#include "cmw-cmx-cpp/ProcessComponent.h"
#include "cmw-cmx-cpp/Component.h"


static const double R = 50;
static double averageInterval = 0.0;

class mBeanClass
{
public:
	mBeanClass(): _shotCmp(cmw::cmx::Component::create("shotComponent")),
				  _shotCount(_shotCmp->newInt64("shotCount")),
				  _failShotCount(_shotCmp->newInt64("failShotCount")),
				  _successShotCount(_shotCmp->newInt64("successShotCount")),
				  _fourFails(_shotCmp->newString("fourFails", 18)),
				  _visibleAreaOutside(_shotCmp->newString("visibleAreaOutside", 20)),
				  _devidedByFive(_shotCmp->newString("devidedByFive", 15)),
				  _successPercent(_shotCmp->newFloat64("successPercent")),
				  _averageInterval(_shotCmp->newFloat64("averageInterval"))
	{
		_shotCount = 0;
		_failShotCount = 0;
		_successShotCount = 0;
		_countFails = 0;
		_successPercent = 0.0;
		_fourFails = "";
		_visibleAreaOutside = "";
		_devidedByFive = "";
		_averageInterval = 0.0;
	}

	void mBeanCheck(const std::pair<double, double> &shot)
	{
		_shotCount = _shotCount+1;
		_averageInterval = averageInterval;
		if(pow(shot.first*shot.first+shot.second*shot.second, 0.5) <= R)
		{
			_successShotCount = _successShotCount+1;
			_countFails = 0;
			_fourFails = "";
		}
		else
		{
			_failShotCount = _failShotCount+1;
			++_countFails;
		}
		if (_countFails >= 4)
			_fourFails = "four fail";

		checkShotInAria(shot);
		checkByFive();
		findSuccessPercent();
	}

private:
	void checkShotInAria(const std::pair<double, double> &shot)
	{
		if(pow(shot.first*shot.first+shot.second*shot.second, 0.5) >= 2*R)
			_visibleAreaOutside = "Outside visible area";
		else
			_visibleAreaOutside = "";
	}

	void checkByFive()
	{
		if(_shotCount % 5 == 0)
			_devidedByFive = "Devided by five";
		else
			_devidedByFive = "";
	}

	void findSuccessPercent()
	{
		_successPercent = float(_successShotCount)/float(_shotCount);
	};

private:
	cmw::cmx::ComponentPtr _shotCmp;
	cmw::cmx::CmxInt64     _shotCount;
	cmw::cmx::CmxInt64     _failShotCount;
	cmw::cmx::CmxInt64     _successShotCount;
	cmw::cmx::CmxString    _fourFails;
	cmw::cmx::CmxString    _visibleAreaOutside;
	cmw::cmx::CmxString    _devidedByFive;
	cmw::cmx::CmxFloat64   _successPercent;
	cmw::cmx::CmxFloat64   _averageInterval;

	int _countFails;
};

class SomeClass
{
public:
	explicit SomeClass(std::vector<std::pair<double, double>> &shotsVector): shotsVector(shotsVector){};

	static void checkShot(const std::pair<double, double> &shot)
	{
		if(pow(shot.first*shot.first+shot.second*shot.second, 0.5) <= R)
			std::cout << "Попадание!" << std::endl;
		else
			std::cout << "Промах!" << std::endl;
	}

	void findAverageInterval(const std::pair<double, double> &shot)
	{
		for(const auto & i : shotsVector)
		{
			double interval = countInterval(i.first, i.second, shot.first, shot.second);
			averageInterval = countAverageInterval(interval);
		}
	}

private:
	double countInterval(double x1, double y1, double x2, double y2)
	{
		return pow(pow((x1 - x2), 2)+pow((y1 - y2), 2), 0.5);
	}

	double countAverageInterval(double interval)
	{
		return (averageInterval*int(intervalsCount) + interval)/double(++intervalsCount);
	}

private:
	int intervalsCount = 0;
	std::vector<std::pair<double, double>> &shotsVector;
};




int main()
{
	cmw::cmx::ProcessComponent::update();
	mBeanClass mBean;
	std::vector<std::pair<double, double>> shotsVector;
	SomeClass someClass(shotsVector);

	bool isAlive = true;
	std::thread thread2([&isAlive]()
	{
		sleep(20);
		isAlive = false;
	});
	thread2.detach();

	std::ifstream in("../test.txt");
	std::cin.rdbuf(in.rdbuf());


	std::pair<double, double> shot;
	while(isAlive)
	{
//		std::cout << std::endl;
//		std::cout << "Совершите выстрел:" << std::endl;
		std::cin >> shot.first >> shot.second;
		SomeClass::checkShot(shot);
		someClass.findAverageInterval(shot);
		shotsVector.push_back(shot);
		mBean.mBeanCheck(shot);
		usleep(100000);
	}

	return 0;
}
