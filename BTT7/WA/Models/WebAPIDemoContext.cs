using Microsoft.EntityFrameworkCore;
using System.Collections.Generic;

namespace WA.Models
{
    public class WebAPIDemoContext : DbContext
    {
        public WebAPIDemoContext(DbContextOptions<WebAPIDemoContext> options) : base(options) { }
        public DbSet<WA.Models.SensorData> SensorDatas { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlite("Data Source=Application.db;Cache=Shared");
            base.OnConfiguring(optionsBuilder);

        }
    }
}
