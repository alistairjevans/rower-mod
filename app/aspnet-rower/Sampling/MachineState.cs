using System;

namespace aspnetrower
{

  public class MachineState : IMachineState
  {
    private DateTime? startTime;
    private uint firstTimeStamp;
    private uint lastTimeStamp;

    private int zeroedSeatCount;
    private int currentSensorSeatCount;

    public DateTime LastSample { get; private set; }

    public int Count => (currentSensorSeatCount - zeroedSeatCount);

    public double Speed { get; private set; }

    public void UpdateMachineState(uint sensorMilliseconds, double speed, int seatCount)
    {
      // Determine our milliseconds offset from the sensor, so we can sort of 
      // sync our 'now' with the sensor clock.
      if (startTime == null || sensorMilliseconds < lastTimeStamp)
      {
        startTime = DateTime.Now;
        firstTimeStamp = sensorMilliseconds;
        zeroedSeatCount = seatCount;
      }

      var determinedSampleTime = startTime.Value.AddMilliseconds(sensorMilliseconds - firstTimeStamp);

      if(Math.Abs((DateTime.Now - determinedSampleTime).TotalSeconds) > 5)
      {
        // Compensate for drift, allowing the sensor time to be reset if need be.
        firstTimeStamp = sensorMilliseconds;
      }

      // We're going to 'pull down' the speed to zero if it is less than 4. The nature of the sensor means that
      // values less than 4 seems to just be the idle rotation of the wheel, rather than being under any force.
      if(speed < 4)
      {
        speed = 0;
      }

      LastSample = startTime.Value.AddMilliseconds(sensorMilliseconds - firstTimeStamp);
      Speed = speed;
      currentSensorSeatCount = seatCount;
      lastTimeStamp = sensorMilliseconds;
    }

    public void ZeroCount()
    {
      zeroedSeatCount = currentSensorSeatCount;
    }
  }
}
