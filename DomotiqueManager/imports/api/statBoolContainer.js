import { Mongo } from 'meteor/mongo';
 
export const StatContainer = new Mongo.Collection('statBoolContainer');

if (Meteor.isServer) {
   Meteor.publish('statBoolContainer', function statContainerPublication(collectionName) {
    console.log('PUBLISHING ' +collectionName)
     return StatContainer.find({statIdentifier: collectionName})
  });
}