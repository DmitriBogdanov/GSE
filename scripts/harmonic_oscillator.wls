#!/usr/bin/env wolframscript
(* ::Package:: *)

(* Import data *)
json=Import[NotebookDirectory[]<>"../temp/harmonic_oscillator.json"];

solution="solution"/.json;
t="t"/.solution;
y="x"/.solution;
x=y[[All,1]];
v=y[[All,2]];

(* Visualize *)
style={PlotStyle->Directive[Lighter@Black],AxesStyle->Directive[Black,Thick],LabelStyle->Directive[Black,Bold],ImageSize->Medium,Joined->True,Mesh->All,MeshStyle->Directive[Red, PointSize@Medium],PlotRange->Full};

grid=Grid[{
{
(* 'x' component *) ListPlot[Transpose@{t,x},Evaluate@style,AxesLabel->{"t","x(t)"},PlotLabel->"Coordinate"],
(* 'v' component *) ListPlot[Transpose@{t,v},Evaluate@style,AxesLabel->{"t","v(t)"},PlotLabel->"Speed"]
},
{
(* Phase plot *) ListPlot[y,Evaluate@style,AxesLabel->{"x","v"},PlotLabel->"Phase Trajectory"],
(* Energy plot *) ListPlot[Transpose@{t,Power[v,2]},Evaluate@style,AxesLabel->{"t","\!\(\*FractionBox[SuperscriptBox[\(mv\), \(2\)], \(2\)]\)"},PlotLabel->"Kinetic Energy"]
}
},Background->White,Frame->All,Spacings->{2,2}]

Export[NotebookDirectory[]<>"images/ode_harmonic_oscillator.svg",grid]




