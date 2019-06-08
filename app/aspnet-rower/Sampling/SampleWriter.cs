using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace aspnetrower
{

  public class SampleWriter : ISampleWriter, IDisposable
  {
    private readonly string folder;
    private readonly string prefix;
    private StreamWriter streamWriter;
    private bool startNew = true;

    public SampleWriter(string folder, string prefix)
    {
      this.folder = folder;
      this.prefix = prefix;

      if(!Directory.Exists(folder))
      {
        Directory.CreateDirectory(folder);
      }
    }

    public void Dispose()
    {
      if (streamWriter != null)
      {
        streamWriter.Dispose();
      }
    }

    public void StartNewFile()
    {
      startNew = true;
    }

    public async Task ProvideSample(DateTime timestamp, double speed, int count)
    {
      if(startNew)
      {
        if(streamWriter != null)
        {
          streamWriter.Dispose();
        }
        var filePath = Path.Combine(folder, $"{prefix}_{timestamp:dd-MM-yy-HH-mm}.csv");
        // Allow appending within the same minute.
        streamWriter = new StreamWriter(File.Open(filePath, FileMode.Append, FileAccess.Write, FileShare.Read));
        streamWriter.WriteLine("time,speed,count");
        startNew = false;
      }

      await streamWriter.WriteLineAsync($"{timestamp:o},{speed},{count}");
    }
  }
}
