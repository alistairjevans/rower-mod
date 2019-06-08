using System.Threading.Tasks;
using aspnetrower.Hubs;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.SignalR;

namespace aspnetrower.Controllers
{
  public class DataController : Controller
  {
    private readonly IMachineState machineState;
    private readonly ISampleWriter sampleWriter;
    private readonly IHubContext<FeedHub> feedHub;

    public DataController(IMachineState machineState, ISampleWriter sampleWriter, IHubContext<FeedHub> feedHub)
    {
      this.machineState = machineState;
      this.sampleWriter = sampleWriter;
      this.feedHub = feedHub;
    }

    [HttpPost]
    public async Task<ActionResult> ProvideReading(uint milliseconds, double speed, int count)
    {
      // Update our machine state.
      machineState.UpdateMachineState(milliseconds, speed, count);

      // Write the sample to file (our sample writer) and update all clients
      // Wait for them both to finish.
      await Task.WhenAll(
        sampleWriter.ProvideSample(machineState.LastSample, machineState.Speed, machineState.Count),
        feedHub.Clients.All.SendAsync("newData", 
                                      machineState.LastSample.ToString("o"), 
                                      machineState.Speed, 
                                      machineState.Count)
      );

      return StatusCode(200);
    }
  }
}
