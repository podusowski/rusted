#pragma once

namespace Cake
{
namespace Threading
{

class IRunnable
{
public:
	virtual void run() = 0;
};

}
}
