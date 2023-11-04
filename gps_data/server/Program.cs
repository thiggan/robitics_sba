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
builder.Services.AddSingleton<Store>(new Store());

var app = builder.Build();

app.UseCors();

app.MapGet("/", () => "Hello World!");

app.MapGet("/api/gps", async (Store store) =>
{
    return Results.Json(store.Current);
});

app.Run();

public record Gps(double Latitude, double Longitude, double Altitude, double speed, double Angle, double Satellites, string FixType, DateTime TimeStamp)
{
    public static Gps Random()
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

        return new Gps(randomLat, randomLon, r.Next(-20, 200), r.Next(0, 100), r.Next(0, 364), r.Next(0, 20), "todo", DateTime.UtcNow);
    }
}

public class Store
{
    public Gps Current { get; set; }
}

public class DevicePullerBackgroundTask : BackgroundService
{
    readonly TimeSpan _period = TimeSpan.FromSeconds(1);
    private readonly Store store;

    public DevicePullerBackgroundTask(Store store)
    {
        this.store = store;
    }

    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        using var timer = new PeriodicTimer(_period);
        while(! stoppingToken.IsCancellationRequested
            && await timer.WaitForNextTickAsync(stoppingToken))
        {
            store.Current = Gps.Random();
        }
    }
}