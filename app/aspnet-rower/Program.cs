using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using EnvironmentName = Microsoft.Extensions.Hosting.EnvironmentName;

namespace aspnetrower
{
  public class Program
  {
    public static void Main(string[] args)
    {
      CreateHostBuilder(args).Build().Run();
    }

    public static IHostBuilder CreateHostBuilder(string[] args) {
    
      var currentDirectory = Directory.GetCurrentDirectory();
      var environment = Environment.GetEnvironmentVariable("ASPNETCORE_ENVIRONMENT");
      var isDevelopment = environment == EnvironmentName.Development;

      return Host.CreateDefaultBuilder(args)
              .ConfigureWebHostDefaults(webBuilder =>
              {
                webBuilder.UseUrls("http://localhost:5000", $"http://{Environment.MachineName}:5000/");
                webBuilder.UseStartup<Startup>();
                if(!isDevelopment)
                {
                  webBuilder.UseWebRoot("wwwroot");
                }

              });
    }
  }
}
