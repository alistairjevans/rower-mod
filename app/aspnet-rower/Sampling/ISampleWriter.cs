using System;
using System.Threading.Tasks;

namespace aspnetrower
{
  public interface ISampleWriter
  {
    Task ProvideSample(DateTime timestamp, double speed, int count);

    void StartNewFile();
  }
}
