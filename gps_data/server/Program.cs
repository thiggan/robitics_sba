var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

app.MapGet("/", () => "Hello World!");

app.MapGet("/api/gps", async () => {
    return Results.Json(Gps.Random());
});

app.Run();

record Gps ( double Latitude, double Longitude, double Altitude, double speed, double Angle, double Satellites, string FixType, DateTime TimeStamp)
{
    public static Gps Random()
    {
        // generate a random gps within a pretend rectangle over 'greater dunedin'
        // this is tempoary untill i figure out how to connect this to a com-port
        // and return the data we get from the hardware.

        var lat1 = -45.83263923190547;
        var lon1 = 170.3443013870225;
        var lat2 = -45.93182383670917;
        var lon2 = 170.3443013870225;

        var r = new Random();

        var randomLat = r.NextDouble() * (Math.Max(lat1, lat2) - Math.Min(lat1, lat2)) + Math.Min(lat1, lat2);
        var randomLon = r.NextDouble() * (Math.Max(lon1, lon2) - Math.Min(lon1, lon2)) + Math.Min(lon1, lon2);

        return new Gps(randomLat, randomLon, r.Next(-20, 200), r.Next(0, 100), r.Next(0, 364), r.Next(0, 20), "todo", DateTime.UtcNow);
    }
}