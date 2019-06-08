using Microsoft.AspNetCore.SignalR;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace aspnetrower.Hubs
{
  public class FeedHub : Hub
  {
    private readonly IMachineState machineState;
    private readonly ISampleWriter sampleWriter;

    public FeedHub(IMachineState machineState, ISampleWriter sampleWriter)
    {
      this.machineState = machineState;
      this.sampleWriter = sampleWriter;
    }

    public void ResetCount()
    {
      machineState.ZeroCount();
      sampleWriter.StartNewFile();
    }
  }
}
