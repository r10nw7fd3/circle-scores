# circle-scores

Get notified about 800pp+ scores by players from desired leaderboard page

## Building

Install the following dependencies:

- libcurl
- rapidjson

Run

```
make -j$(nproc)
```

## Running

Create `credentials.json`:

```
{
	"osu_id": (osu api id),
	"osu_key": "(osu api key)",
	"discord_hook_url": "(discord hook url, optional)",
	"telegram_bot_token": "(telegram bot token, optional)"
}
```

Copy `config.json.default` to `config.json` and modify it according to your needs

Run

```
./circle-scores
```

to start the thing or run

```
./circle-scores -h
```

to see available arguments
