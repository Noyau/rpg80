using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace rpg80
{
    public static class Program
    {
        #region TREE
        [System.Serializable]
        public sealed class Tree
        {
            public List<Node> nodes = new List<Node> { };
            public List<Edge> edges = new List<Edge> { };
        } // class: Tree
        #endregion TREE

        #region EDGE
        [System.Serializable]
        public struct Edge
        {
            public int source;
            public int target;

            public Edge(int source, int target)
            {
                this.source = source;
                this.target = target;
            }
        } // struct: Edge
        #endregion EDGE

        #region NODE
        [System.Serializable]
        public abstract class Node
        { } // class: Node

        [System.Serializable]
        public sealed class NarrativeNode : Node
        {
            public string text;

            public NarrativeNode(string text)
            {
                this.text = text;
            }
        } // class: NarrativeNode

        [System.Serializable]
        public sealed class DialogNode : Node
        {
            public string characterName;
            public string characterDescription;
            public string characterText;

            public DialogNode(string cName, string cDescription, string cText)
            {
                this.characterName = cName;
                this.characterDescription = cDescription;
                this.characterText = cText;
            }
        } // class: DialogNode

        [System.Serializable]
        public sealed class CombatNode : Node
        {
            public string enemyName;
            public string enemyDescription;
            public string enemyText;

            public CombatNode(string eName, string eDescription, string eText)
            {
                this.enemyName = eName;
                this.enemyDescription = eDescription;
                this.enemyText = eText;
            }
        } // class: CombatNode
        #endregion NODE

        private static void OnNextNode(Node node)
        {
            if (node is NarrativeNode)
                OnNextNarrative(node as NarrativeNode);
            else if (node is DialogNode)
                OnNextDialog(node as DialogNode);
            else if (node is CombatNode)
                OnNextCombat(node as CombatNode);
        }

        private static void OnNextNarrative(NarrativeNode node) => Console.WriteLine(node.text); // TODO
        private static void OnNextDialog(DialogNode node)
        {
            StringBuilder _text = new StringBuilder();
            _text.AppendLine($"[{node.characterName}] ({node.characterDescription})");
            _text.Append($"[{node.characterName}] {node.characterText}");
            Console.WriteLine(_text.ToString());
            // TODO
        }
        private static void OnNextCombat(CombatNode node)
        {
            StringBuilder _text = new StringBuilder();
            _text.AppendLine($"[{node.enemyName}]");
            _text.AppendLine($"    {node.enemyDescription}");
            _text.Append($"-> {node.enemyText}");
            Console.WriteLine(_text.ToString());
            // TODO
        }

        public static void Main(string[] args)
        {
            // Just a simple test... ahem...
            // So many things to do...
            Tree _tree = new Tree
            {
                nodes = new List<Node>
                {
                    new NarrativeNode("Bonjour aventurier !"),
                    new NarrativeNode("Droit devant vous se tient un vieil homme, parlez lui."),
                    new DialogNode("Pedro Bear", "Porte un grand manteau mais pas de... pantalon ?", "Tu as déjà vu un monsieur tout nu ?"),
                    new NarrativeNode("Soudain le vieil homme ouvre son grand manteau d'un geste si vif que vous parvenez à peine à le distinguer..."),
                    new CombatNode("Pedro « Maître Biffleur » Bear", "Ah le vicieux ! Il est vif !", "L'ennemie a l'initiative et vous inflige une « Méga-biffle des Bois », tout devient noir..."),
                    new NarrativeNode("La légende raconte que des fantômes hantent les lieux et dévorent l'âme des imprudents."),
                    new NarrativeNode("Mais vous vous connaissez la vérité."),
                    new NarrativeNode("Malheureusement vous êtes morts."),
                    new NarrativeNode("Merci d'avoir joué."),
                    new NarrativeNode("Bisous"),
                },
                edges = new List<Edge>
                {
                    new Edge(0, 1),
                    new Edge(1, 2),
                    new Edge(2, 3),
                    new Edge(3, 4),
                    new Edge(4, 5),
                    new Edge(5, 6),
                    new Edge(6, 7),
                    new Edge(7, 8),
                    new Edge(8, 9),
                }
            };

            bool _quit = false;

            Console.WriteLine("RPG-80");
            Console.WriteLine("Vous pouvez quitter le programme à tout moment en appuyant sur la touche (plan) 'Q'.");
            Console.WriteLine("(Mais en vrai, faites ALT+F4, on se sens plus puissant quand on a droit de vie ou de mort sur les processus.)");
            Console.WriteLine("Préparez vous à une aventure sans pareil !");
            Console.WriteLine("------------------------------------------");

            int _idx = 0;

            do
            {
                // Load node
                Node _node = _tree.nodes[_idx];

                OnNextNode(_node);

                char _key = Console.ReadKey(false).KeyChar;

                if (_key != 'q' && _key != 'Q')
                {
                    // Find next node
                    try
                    {
                        _idx = _tree.edges.First(_e => _e.source == _idx).target;
                        // Well... do not use Linq kids, btw...
                    }
                    catch
                    {
                        Console.WriteLine("------------------------------------------");
                        string _response;
                        do
                        {
                            Console.WriteLine("Souhaitez-vous continuer ? [O]ui/[n]on");
                            _response = Console.ReadLine().ToLower();
                            if (_response.StartsWith("q") || _response.StartsWith("n"))
                                _quit = true;
                            else if (!_response.StartsWith("o"))
                                _response = string.Empty;
                        } while (string.IsNullOrEmpty(_response) && !_quit);

                        Console.WriteLine("Ok");

                        if (!_quit)
                        {
                            _idx = 0;
                            Console.WriteLine("------------------------------------------");
                        }
                    }
                }
                else
                {
                    _quit = true;
                }
            } while (!_quit);

            Console.WriteLine("------------------------------------------");
            Console.WriteLine("À bientôt.");
            Console.ReadKey();
        }
    } // class: Program
} // namespace