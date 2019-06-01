/**
 * @file timer_stop.hpp
 * @author Marcus Edel
 *
 * Implementation of the timer stop callback function.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef ENSMALLEN_CALLBACKS_TIMER_STOP_HPP
#define ENSMALLEN_CALLBACKS_TIMER_STOP_HPP

namespace ens {

/**
 * Timer stop function, based on the EndEpoch callback function.
 */
class TimerStop
{
 public:
  /**
   * Set up the print loss callback class with the width and output stream.
   *
   * @param time The timeout in seconds.
   */
  TimerStop(const double time) : time(time), reset(false)
  { /* Nothing to do here. */ }

  template<typename OptimizerType, typename FunctionType, typename MatType>
  void BeginEpoch(OptimizerType& /* optimizer */,
                  FunctionType& /* function */,
                  const MatType& /* coordinates */,
                  const size_t /* epoch */,
                  const double /* objective */)
  {
    // Start the timer.
    if (!reset)
    {
      timer.tic();
      reset = true;
    }
  }

  /**
   * Callback function called at the end of a pass over the data.
   *
   * @param optimizer The optimizer used to update the function.
   * @param function Function to optimize.
   * @param coordinates Starting point.
   * @param epoch The index of the current epoch.
   * @param objective Objective value of the current point.
   */
  template<typename OptimizerType, typename FunctionType, typename MatType>
  void EndEpoch(OptimizerType& optimizer,
                FunctionType& /* function */,
                const MatType& /* coordinates */,
                const size_t /* epoch */,
                const double /* objective */)
  {
    if (timer.toc() > time)
    {
      Info << "Timer timeout reached; terminate optimization." << std::endl;
      optimizer->Terminate() = true;
    }
  }

 private:
  //! The timeout in seconds.
  double time;

  //! Reset the timer.
  bool reset;

  //! Locally-stored timer object.
  arma::wall_clock timer;
};

} // namespace ens

#endif
