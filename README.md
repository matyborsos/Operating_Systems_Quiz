# Operating_Systems_Quiz
Quiz game I implemented for one of the assignments for the Operating Systems course.

# Description
Program that writes multiple choice questions to the standard output and then reads answers from the standard input until the correct answer has been given or the number of attempts to provide an answer has been exceeded. The game repeats until the user ends the standard input (by pressing CTRL-D on Linux/MacOS or CTRL-Z on Windows). The game then shows the final score.

Your program obtains the questions from a web service. Simply run a tool like curl to fetch questions and you parse the output produced by curl. An example invocation of curl in your shell may look like this:

$ curl -s 'https://opentdb.com/api.php?amount=1&category=18&type=multiple'
{
  "response_code": 0,
  "results": [
    {
      "category": "Science: Computers",
      "type": "multiple",
      "difficulty": "medium",
      "question": "What was the name given to Android 4.3?",
      "correct_answer": "Jelly Bean",
      "incorrect_answers": [
        "Lollipop",
        "Nutella",
        "Froyo"
    ]
    }
  ] 
}

The URL requests a single multiple-choice question from the computer science category 18. If the request is successful, the curl program returns a JSON document, which includes the question, the correct answer, as well as several wrong answers.


