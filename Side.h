#ifndef Side_h
#define Side_h
#include <limits>
enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

const int PINF = std::numeric_limits<int>::max();
const int NINF = std::numeric_limits<int>::min();

//========================================================================
// Timer t;                 // create and start a timer
// t.start();               // restart the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

//========================================================================
// Advance jumpy timer only after jumpInterval calls to elapsed
class JumpyTimer
{
public:
  JumpyTimer(int jumpInterval)
   : m_jumpInterval(jumpInterval), m_callsMade(0)
  {
      actualElapsed();
  }
  double elapsed()
  {
      m_callsMade++;
      if (m_callsMade == m_jumpInterval)
      {
          m_lastElapsed = m_timer.elapsed();
          m_callsMade = 0;
      }
      return m_lastElapsed;
  }
  double actualElapsed()
  {
      m_lastElapsed = m_timer.elapsed();
      return m_lastElapsed;
  }
private:
  Timer m_timer;
  int m_jumpInterval;
  int m_callsMade;
  int m_lastElapsed;
};


#endif /* Side_h */

