using System.Globalization;
using System.IO.Ports;
using System.Text.Json;
using System.Text.Json.Serialization;

var MyAllowSpecificOrigins = "_myAllowSpecificOrigins";

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddCors(options =>
{
    options.AddDefaultPolicy(
                      policy =>
                      {
                          policy.AllowAnyOrigin();
                          policy.AllowAnyMethod();
                          policy.AllowAnyHeader();
                      });
});
builder.Services.AddHostedService<DevicePullerBackgroundTask>();
builder.Services.AddSingleton(new Store());
builder.Services.AddSingleton(new GpsReader());

var app = builder.Build();

app.UseCors();

app.MapGet("/", () => "Hello World!");

app.MapGet("/api/gps", async (Store store) =>
{
    return Results.Json(store.Current);
});

app.Run();

public class FixInformation
{
    [JsonPropertyName("message")] public string Message { get; set; }
    [JsonPropertyName("latitude")] public double Latitude { get; set; }
    [JsonPropertyName("longitude")] public double Longitude { get; set; }
    [JsonPropertyName("altitude")] public double Altitude { get; set; }
    [JsonPropertyName("speed")] public double Speed { get; set; }
    [JsonPropertyName("angle")] public double Angle { get; set; }
    [JsonPropertyName("fix")] public int Fix { get; set; }
    [JsonPropertyName("fixquality")] public int FixQuality { get; set; }
    [JsonPropertyName("satellites")] public int Satellites { get; set; }
    [JsonPropertyName("antenna")] public int Antenna { get; set; }
    [JsonPropertyName("timestamp")] public string TimeStamp { get; set; }


    public static FixInformation Random()
    {
        // generate a random gps within a pretend rectangle over 'greater dunedin'
        // this is tempoary untill i figure out how to connect this to a com-port
        // and return the data we get from the hardware.

        var lat1 = -45.592785; 
        var lon1 = 169.663514;
        var lat2 = -46.047783;
        var lon2 = 171.180190;

        var r = new Random();

        var randomLat = r.NextDouble() * (Math.Max(lat1, lat2) - Math.Min(lat1, lat2)) + Math.Min(lat1, lat2);
        var randomLon = r.NextDouble() * (Math.Max(lon1, lon2) - Math.Min(lon1, lon2)) + Math.Min(lon1, lon2);

        return new FixInformation()
        {
            Message = "fix",
            Latitude = randomLat,
            Longitude = randomLon,
            Altitude = r.Next(-20, 200),
            Speed = r.Next(0, 100),
            Angle = r.Next(0, 364),
            Satellites = r.Next(0, 20),
        };
    }
}
public class GpsError
{
    public string err { get; set; }
}

public class Store
{
    public FixInformation Current { get; set; }
}

public class DevicePullerBackgroundTask : BackgroundService
{
    readonly TimeSpan _period = TimeSpan.FromSeconds(5);
    private readonly Store store;
    private readonly GpsReader reader;

    public DevicePullerBackgroundTask(Store store, GpsReader reader)
    {
        this.store = store;
        this.reader = reader;
        this.reader.Start();
    }

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        using var timer = new PeriodicTimer(_period);
        while(! stoppingToken.IsCancellationRequested
            && await timer.WaitForNextTickAsync(stoppingToken))
        {
            //store.Current = FixInformation.Random();
            var x = JsonSerializer.Serialize(FixInformation.Random());

            store.Current = reader.Current;
        }
    }
}

public class GpsReader
{
    SerialPort port = null;

    public void Start()
    {
        port = new SerialPort("COM7", 115200);
        port.Handshake = Handshake.None;
        port.ReadTimeout = (int)TimeSpan.FromSeconds(10).Ticks;
        port.DataReceived += Port_DataReceived;
        port.Open();
    }

    private void Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
    {
        var data = port.ReadLine();

        if (data.Contains("err"))
        {
            var err = JsonSerializer.Deserialize<GpsError>(data);
            Current = new FixInformation()
            {
                Message = err.err
            };
            HasError = true;
        }
        else if(data.Contains("latitude"))
        {
            
            var fix = JsonSerializer.Deserialize<FixInformation>(data);
            fix.Message = "fix";
            Current = fix;
            HasError = false;
        }
        else 
        {
            Current = Current = new FixInformation()
            {
                Message = "unable to process fix information"
            };
            HasError = true;
        }
    }

    public void Stop()
    {
        if (port != null)
        {
            port.Dispose();
            port = null;
        }
    }

    public FixInformation Current { get; private set; }
    public bool HasError { get; private set; }
}