# yedit
that's right! i'm rewriting yedit again! (i've done this 14 times now i don't know when to stop)

## compiling
`make`

## running
run bin/yedit

## The UI
<img src="pic/clock.png">
this part of the UI shows you the time<br><br>
<img src="pic/filename.png">
this part of the UI shows you the filename/path of the currently open file<br><br>
<img src="pic/yeditcurp.png">
this part of the UI shows you the position of your cursor<br><br>
<img src="pic/yeditlines.png">
this part of the UI shows you how many lines are in the currently opened file<br><br>

## Keys
| Key      | Actions             |
| -------- | --------------------|
| Ctrl + S | Save                |
| Ctrl + W | Save As             |
| Ctrl + O | Open                |
| Ctrl + T | Suspend to terminal |
| Ctrl + Q | Quit                |
| Ctrl + F | Find                |
| Ctrl + E | Run command         |
| Pg Dn    | Scroll down 4 lines |
| Pg Up    | Scroll up 4 lines   |

## Commands
right now commands are only for debug, but here's a list
| Command              | What it does                      |
| -------------------- | --------------------------------- |
| toggle render null   | renders null terminators as ^@    |
