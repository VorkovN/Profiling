#include <iostream>
#include <thread>
#include <cmath>
#include <vector>

#include "cmw-cmx-cpp/ProcessComponent.h"
#include "cmw-cmx-cpp/Component.h"
#include "cmw-cmx/common.h"


static const double R = 10;

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
		cmw::cmx::ProcessComponent::update();
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
		_shots.push_back(shot);

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
		findAverageInterval();
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

	void findAverageInterval()
	{
		int intervalsCount = 0;
		for(int i = 0; i < _shots.size()-1; ++i)
			for(int j = i+1; j < _shots.size(); ++j )
			{
				double interval = pow(pow((_shots[i].first-_shots[j].first), 2)+pow((_shots[i].second-_shots[j].second), 2), 0.5);
				_averageInterval = (_averageInterval*int(intervalsCount) + interval)/double(intervalsCount + 1);
				++intervalsCount;
			}
	}

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
	unsigned long long _lastShotTime;

	std::vector<std::pair<double, double>> _shots;
};


void checkShot(const std::pair<double, double> &shot)
{
	if(pow(shot.first*shot.first+shot.second*shot.second, 0.5) <= R)
		std::cout << "Попадание!" << std::endl;
	else
		std::cout << "Промах!" << std::endl;
}

int main()
{
	mBeanClass mBean;

	bool isAlive = true;
	std::thread([&isAlive]()
	{
		sleep(120);
		isAlive = false;
	}).detach();

	std::pair<double, double> shot;
	while(isAlive)
	{
		std::cout << "Совершите выстрел:" << std::endl;
		std::cin >> shot.first >> shot.second;
		checkShot(shot);
		std::cout << std::endl;
		mBean.mBeanCheck(shot);
	}

	return 0;
}
