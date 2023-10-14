# circle-scores

Get notified about 800pp+ scores by players from desired leaderboard page

## Building

Install the following dependencies:

- libcurl
- rapidjson

Run

```
make
```

## Running

`credentials.json` format:

```
{
	"osu_id": (osu api id),
	"osu_key": "(osu api key)",
	"osu_legacy_key": "(osu api v1 key, optional)",
	"discord_hook_url": "(discord hook url, optional)"
}
```

Run

```
./circle-scores -h
```

To see available arguments
