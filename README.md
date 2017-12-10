# ggClassyArchitect

UML-like architecture tool. QT project heavily using QGraphicsItem.

![](Classy%20Architect%20Screen%20Shot.png)

The user can enter a list of **classes**, base-classes, class-members, and class-description. This list can be very long, and heavily linked. Classes may be grouped in different **collections**, whereas each collection has it's own color-style. The ggClassyArchitect does not show ALL of the classes and ALL its connections by default in its view, but the user can select a set of classes for visualisation. The same class can be visualized with multiple **class-boxes** in the same view. The viewer displays only the shortest connection lines (this allows to draw charts with less tangled connections). **Multiple views** based on the same set of classes can be created, showing different aspects of an architecture. Class properties can be edited interactively within the view.

## Development: Goal
- As simple and powerful as Google Drawings
- With fully automatic connection lines between classes and members
- Collections of classes with same color-style

## Development: Next Steps ToDo
- Load Data Set (from file)
- Interactive delete Class-Box (just the box and not the class, "x" on top right)
- Docking Tool-Bar (list classes, add/delete classes, add/delete class-boxes)
- Class-Box: edit members, and member-class-names
- Docking Tool-Bar (collections, edit colors and fonts, line-styles)
- Frames (rectangular free-hand boxes for visually grouping class-boxes)
- Docking Tool-Bar (frames color properties)
- Menu with default preferences (for new class-boxes, or frames)
- Menu with recenr file list
- Menu with undo/redo
- Multiple views (different sets of class-boxes)
- etc.

## Development: Components Dependency
![](Classy%20Architect%20Components%20Dependency.svg)
https://docs.google.com/drawings/d/1QtExo6Zlpknl1kIhxFEQ5TYR7_OOUjRvJf9V4tQW1Qc
