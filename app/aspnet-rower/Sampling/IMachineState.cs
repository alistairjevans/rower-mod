using System;

namespace aspnetrower
{
  public interface IMachineState
  {
    DateTime LastSample { get; }
    int Count { get; }
    double Speed { get; }

    void UpdateMachineState(uint sensorMilliseconds, double speed, int seatCount);

    void ZeroCount();
  }
}
