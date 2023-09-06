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

`credentials.txt` format:

```
osu api v2 id
osu api v2 key
osu api v1 key
discord hook url
```

Run

```
./circle-scores -h
```

To see available arguments
