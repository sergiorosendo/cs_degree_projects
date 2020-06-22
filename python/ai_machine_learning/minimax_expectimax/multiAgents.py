# Aluno: Sergio Rosendo da Silva Junior - NUSP: 6508702
# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

def foodCount(foods):
  count = 0
  for food in foods:
    if food == True:
      count += 1
  return count

def closerToFoodPoints(currentPos, foods):
  greaterDist = -1
  for foodPos in foods.asList():
    dist = manhattanDistance(currentPos, foodPos)
    if greaterDist == -1 or dist > greaterDist:
      greaterDist = dist
  
  points = 10 + greaterDist
  
  lesserDist = -1
  for foodPos in foods.asList():
    dist = manhattanDistance(currentPos, foodPos)
    if lesserDist == -1 or dist < lesserDist:
      lesserDist = dist
  points -= lesserDist
  
  return points*(-0.25)

def smallestFoodDist(gameState):
  currentPos = gameState.getPacmanPosition()
  foods = gameState.getFood()
  smallestDist = -1
  for foodPos in foods.asList():
    dist = manhattanDistance(currentPos, foodPos)
    if smallestDist == -1 or dist < smallestDist:
      smallestDist = dist
  return smallestDist

def closerToFood(currentGameState, successorGameState):
  if smallestFoodDist(currentGameState) > smallestFoodDist(successorGameState):
    return True
  return False

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]
        
        if successorGameState.isWin(): return float("inf")
        if successorGameState.isLose(): return float("-inf")

        #avoid staying in the same spot
        if successorGameState.getPacmanPosition() == currentGameState.getPacmanPosition():
          return -1
        
        points = 0
        mhDistFromGhost = manhattanDistance(newPos, successorGameState.getGhostPosition(1))
        if mhDistFromGhost < 8:
          points += mhDistFromGhost*4
        if closerToFood(currentGameState, successorGameState):
          points += 25
        if (successorGameState.getNumFood() < currentGameState.getNumFood()): 
          points += 50

        points += successorGameState.getScore()
        return points

def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)    

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        actions = [action for action in gameState.getLegalActions() if action != Directions.STOP]
        maxV = float("-inf")
        bestAction = Directions.STOP

        for action in actions:
          v = self.minValue(gameState.generateSuccessor(0, action), self.depth, 1)
          if v > maxV:
            bestAction = action
            maxV = v

        return bestAction

    def maxValue(self, state, depth):
      """
        Maximum value for the pacman
      """
      if state.isWin() or state.isLose() or depth <= 0: return self.evaluationFunction(state)
        
      v = float("-inf")
      actions = [action for action in state.getLegalActions(0) if action != Directions.STOP]
      nextStates = [state.generateSuccessor(0, action) for action in actions]
      
      for nextState in nextStates:
        v = max(v, self.minValue(nextState, depth, 1))
      
      return v

    def minValue(self, state, depth, agentId):
      """
        Minimum value for the ghosts
      """
      if state.isWin() or state.isLose() or depth <= 0: return self.evaluationFunction(state)
        
      v = float("inf")
      actions = [action for action in state.getLegalActions(agentId) if action != Directions.STOP]
      nextStates = [state.generateSuccessor(agentId, action) for action in actions]
      
      for nextState in nextStates:
        if agentId == (state.getNumAgents() - 1):
          v = min(v, self.maxValue(nextState, depth - 1))
        else:
          v = min(v, self.minValue(nextState, depth, agentId + 1))
      
      return v  
      

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """

        """
        alpha = float("-inf")
        beta = float("inf")
        v = float("-inf")
        bestAction = Directions.STOP
        for action in gameState.getLegalActions():
          v = max(v, self.minValue(gameState.generateSuccessor(0, action), self.depth, 1, alpha, beta))
          if v > alpha: bestAction = action
          alpha = max(alpha, v)

        return bestAction

    def maxValue(self, state, depth, alpha, beta):
      """
        Maximum value for the pacman
      """
      if depth <= 0: return self.evaluationFunction(state)

      v = float("-inf")
      for action in state.getLegalActions():
        v = max(v, self.minValue(state.generateSuccessor(0, action), depth, 1, alpha, beta))
        if v > beta: return v
        alpha = max(alpha, v)

      if v == float("-inf"): return self.evaluationFunction(state)

      return v

    def minValue(self, state, depth, agentId, alpha, beta):
      """
        Minimum value for the ghosts
      """
      if agentId == 0 or agentId == state.getNumAgents():
        return self.maxValue(state, depth - 1, alpha, beta)

      v = float("inf")
      for action in state.getLegalActions(agentId):
        v = min(v, self.minValue(state.generateSuccessor(agentId, action), depth, agentId + 1, alpha, beta))
        if v < alpha: return v
        beta = min(beta, v)

      if v == float("inf"): return self.evaluationFunction(state)

      return v

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    "*** YOUR CODE HERE ***"
    #util.raiseNotDefined()

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    util.raiseNotDefined()

# Abbreviation
better = betterEvaluationFunction

